/*
 * Copyright 2017 Fraunhofer Institute for Manufacturing Engineering and Automation (IPA)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/// @file AbstractColorCamera.h
/// Abstract interface for color cameras.
/// @author Jan Fischer
/// @date May 2008.

#ifndef __IPA_ABSTRACTCOLORCAMERA_H__
#define __IPA_ABSTRACTCOLORCAMERA_H__

#ifdef __LINUX__
	#include "cob_vision_utils/CameraSensorDefines.h"
	#include "cob_vision_utils/CameraSensorTypes.h"
#else
	#include "cob_perception_common/cob_vision_utils/common/include/cob_vision_utils/CameraSensorDefines.h"
	#include "cob_perception_common/cob_vision_utils/common/include/cob_vision_utils/CameraSensorTypes.h"
#endif

#include <boost/shared_ptr.hpp>
#include <sstream>

//#include <opencv2/core/core.hpp>

namespace ipa_CameraSensors {

/// Define smart pointer type for toolbox
class AbstractColorCamera;
typedef boost::shared_ptr<AbstractColorCamera> AbstractColorCameraPtr;

/// An interface for common color cameras.
///	All color/mono cameras that are used within the project
/// must derive from this class to guarantee
/// interoperability with the already existing code.
class __DLL_LIBCAMERASENSORS__ AbstractColorCamera
{
	public:

		/// Struct stores the values from the xml camera configuration file
		/// All values may also be set to AUTO or DEFAULT
		struct t_ColorCameraParameters
		{
			ipa_CameraSensors::t_cameraRole m_CameraRole;	///< Master or slave camera
			std::stringstream m_VideoFormat;				///< Format 0,1,2 or 7
			std::stringstream m_VideoMode;					///< Mode 0-7
			std::stringstream m_ColorMode;					///< Mono8/16S/16, YUV411/422, Raw16, RGB8, ...
			std::stringstream m_IsoSpeed;					///< Guaranteed speed of isochronous transfer rate
			std::stringstream m_FrameRate;
			std::stringstream m_Shutter;
			std::stringstream m_WhiteBalanceU;
			std::stringstream m_WhiteBalanceV;
			std::stringstream m_Hue;
			std::stringstream m_Saturation;
			std::stringstream m_Gamma;
			std::stringstream m_ExposureTime;
			std::stringstream m_Gain;
			std::stringstream m_Brightness;
			std::stringstream m_ImageWidth;
			std::stringstream m_ImageHeight;
			std::stringstream m_Interface;				///< Interface, the camera is connected to (i.e. USB, ETHERNET or FIREWIRE)
			std::stringstream m_IP;						///< IP address of the camera
		};

		/// Initializes the color camera.
		/// Camera specific constants may be set within the configuration file <I>cameraSensorsIni.xml</I>.
		/// The function has to set the member variable <code>m_initialized</code>.
		/// @param directory Path to the configuration file directory.
		/// @param cameraIndex It is possible to have several cameras of the same type on the system.
		///	       One may us the camera index to apply different configuration files to each of them
		/// @return Return code.
		virtual unsigned long Init(std::string directory, int cameraIndex = 0) =0;

		/// Returns true, when <code>Init()</code> has been called on the camera.
		/// @return Camera initialized or not.
		virtual bool isInitialized() {return m_initialized;}

		/// Returns true, when <code>Open()</code> has been called on the camera.
		/// @return Camera opened or not.
		virtual bool isOpen() {return m_open;}

		/// Opens the camera device.
		/// All camera specific parameters for opening the camera should have been set within the <code>Init</code>
		/// function.
		/// @return Return code.
		virtual unsigned long Open() =0;

		/// Close camera device.
		/// @return Return code.
		virtual unsigned long Close() =0; //Save intrinsic params back to File

		/// Retrieves image data from the color camera.
		/// @param colorImageData An array to be filled with image data
		/// @param getLatestFrame True, when the latest picture has to be returned. Otherwise, the next picture
		///						  following the last call to <code>getLatestFrame</code> is returned.
		/// @return Return code
		unsigned long GetColorImage(char* colorImageData, bool getLatestFrame=true) {return RET_FAILED;}

		/// Retrieves an image from the camera.
		/// <code>cv::Mat</code> object is initialized on demand.
		/// @param colorImage The image that has been acquired by the camera.
		/// @param getLatestFrame If true, the camera acquires a new frame and returns it.
		///						  Otherwise, the next frame following the last returned frame
		///						  is returned from the internal camera buffer.
		/// @throw IPA_Exception Throws an exception, if camera access failed
		virtual unsigned long GetColorImage(cv::Mat* colorImage, bool getLatestFrame=true)=0;

		/// Returns the camera type.
		/// @return The camera type
		virtual t_cameraType GetCameraType();

		/// Function to set properties of the camera sensor.
		/// @param propertyID The ID of the property.
		/// @param cameraProperty The value of the property.
		/// @return Return code.
		virtual unsigned long SetProperty(t_cameraProperty* cameraProperty) =0;

		/// Function to set property defaults of the camera sensor.
		/// @return Return code.
		virtual unsigned long SetPropertyDefaults() =0;

		/// Function to get properties of the camera sensor.
		/// @param propertyID The ID of the property.
		/// @param cameraProperty The value of the property.
		/// @return Return code.
		virtual unsigned long GetProperty(t_cameraProperty* cameraProperty) =0;

		/// Displays camera information on standard output.
		/// Information includes available parameters, color and camera formats.
		/// @return Return code.
		virtual unsigned long PrintCameraInformation() =0;

		/// Saves all parameters on hard disk.
		/// @param filename The filename of the storage.
		/// @return Return code.
		virtual unsigned long SaveParameters(const char* filename)=0;

		/// Unit Test for the camera interface.
		/// Tests each of the single interface functions and displays the output on
		/// standard out.
		/// @param filename Path to the camera initialization xml file.
		/// @return Return code.
		virtual unsigned long TestCamera(const char* filename);

		/// Returns the number of images in the directory
		/// @return The number of images in the directory
		virtual int GetNumberOfImages() {return std::numeric_limits<int>::max();};

		/// Function specific to virtual camera.
		/// Resets the image directory read from the configuration file.
		/// @param path The camera path
		/// @return Return code
		virtual unsigned long SetPathToImages(std::string path) {return RET_OK;};

		virtual unsigned long ResetImages() {return RET_OK;};

		/// Destructor
		virtual ~AbstractColorCamera();

	protected:

		bool m_initialized; ///< True, when the camera has sucessfully been initialized.
		bool m_open;		///< True, when the camera has sucessfully been opend.

		t_ColorCameraParameters m_ColorCameraParameters; ///< Storage for xml configuration file data

		t_cameraType m_CameraType; ///< Camera Type

		unsigned int m_BufferSize; ///< Number of images, the camera buffers internally
	private:

		/// Loads all camera specific parameters from the xml configuration file and saves them in t_ColorCameraParameters.
		/// This function is internally called by Init to load the parameters from the xml configuration file.
		/// @param filename The path to the configuration file.
		/// @return Return code.
		virtual unsigned long LoadParameters(const char* filename, int cameraIndex)=0;

		/// Sets the loaded parameters.
		/// @return Return code.
		virtual unsigned long SetParameters()=0;

};

/// Creates, intializes and returns a smart pointer object for the camera.
/// @return Smart pointer, refering to the generated object
__DLL_LIBCAMERASENSORS__ AbstractColorCameraPtr CreateColorCamera_VirtualCam();
__DLL_LIBCAMERASENSORS__ AbstractColorCameraPtr CreateColorCamera_ICCam();
__DLL_LIBCAMERASENSORS__ AbstractColorCameraPtr CreateColorCamera_AxisCam();
__DLL_LIBCAMERASENSORS__ AbstractColorCameraPtr CreateColorCamera_AVTPikeCam();
__DLL_LIBCAMERASENSORS__ AbstractColorCameraPtr CreateColorCamera_OpenCVCamera();
__DLL_LIBCAMERASENSORS__ AbstractColorCameraPtr CreateColorCamera_IDSuEyeCamera();


} // end namespace
#endif // __IPA_ABSTRACTCOLORCAMERA_H__
