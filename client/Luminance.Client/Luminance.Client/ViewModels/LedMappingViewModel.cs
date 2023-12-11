using Camera.MAUI;
using Camera.MAUI.ZXingHelper;
using System.Collections.ObjectModel;
using System.ComponentModel;

namespace Luminance.Client.ViewModels
{
    public class LedMappingViewModel : BaseViewModel
    {
        private CameraView _cameraView;

        public LedMappingViewModel(CameraView _cam)
        {
            _cameraView = _cam;
            _cameraView.CamerasLoaded += CameraLoaded;
        }

        private void CameraLoaded(object? sender, EventArgs e)
        {
            if (_cameraView.NumCamerasDetected == 0)
            {
                return;
            }

            var backCamera = _cameraView.Cameras.FirstOrDefault(x => x.Position == CameraPosition.Back);
            if (backCamera == null)
            {
                return;
            }

            _cameraView.Camera = backCamera;
            MainThread.BeginInvokeOnMainThread(async () =>
            {
                if (await _cameraView.StartCameraAsync() == CameraResult.Success)
                {
                }
            });
        }
    }
}
