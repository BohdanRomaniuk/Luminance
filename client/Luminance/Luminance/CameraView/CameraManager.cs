using System;
using System.Threading.Tasks;
using Microsoft.Maui;
using Microsoft.Maui.ApplicationModel;

namespace Luminance.CameraView
{
    internal partial class CameraManager : IDisposable
    {
        public CameraManager(IMauiContext context, CameraLocation cameraLocation, ICameraView cameraView)
        {
            Context = context;
            CameraLocation = cameraLocation;
            CameraView = cameraView;
        }

        protected readonly IMauiContext Context;
        protected readonly ICameraView CameraView;
        public event EventHandler<CameraFrameBufferEventArgs> FrameReady;

        public CameraLocation CameraLocation { get; private set; }

        public void UpdateCameraLocation(CameraLocation cameraLocation)
        {
            CameraLocation = cameraLocation;

            UpdateCamera();
        }

        public async Task<bool> CheckPermissions()
            => (await Permissions.RequestAsync<Permissions.Camera>()) == PermissionStatus.Granted;

        public void TakePicture() => PlatformTakePicture();

        protected virtual partial void PlatformTakePicture();
    }
}
