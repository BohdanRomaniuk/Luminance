using AndroidX.Camera.Core;
using static Android.Media.Image;

namespace Luminance.CameraView
{
    sealed class ImageCallBack : ImageCapture.OnImageCapturedCallback
    {
        readonly ICameraView cameraView;

        public ImageCallBack(ICameraView cameraView)
        {
            this.cameraView = cameraView;
        }

        public override void OnCaptureSuccess(IImageProxy image)
        {
            base.OnCaptureSuccess(image);
            var img = image.Image;

            if (img is null)
            {
                return;
            }

            var buffer = GetFirstPlane(img.GetPlanes())?.Buffer;

            if (buffer is null)
            {
                image.Close();
                return;
            }

            var imgData = new byte[buffer.Capacity()];
            try
            {
                buffer.Get(imgData);
                var memStream = new MemoryStream(imgData);
                cameraView.OnMediaCaptured(memStream);
            }
            finally
            {
                image.Close();
            }


            static Plane? GetFirstPlane(Plane[]? planes)
            {
                if (planes is null || planes.Length is 0)
                {
                    return null;
                }

                return planes[0];
            }
        }

        public override void OnError(ImageCaptureException exception)
        {
            base.OnError(exception);
            cameraView.OnMediaCapturedFailed();
        }
    }
}
