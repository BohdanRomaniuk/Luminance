using AndroidX.Camera.Core;
using AndroidX.Camera.Lifecycle;
using AndroidX.Camera.View;
using AndroidX.Core.Content;
using Java.Util.Concurrent;
using static Android.App.Usage.NetworkStatsManager;
using static Android.Media.Image;

namespace Luminance.CameraView
{
    internal partial class CameraManager
    {
        AndroidX.Camera.Core.Preview cameraPreview;
        ImageAnalysis imageAnalyzer;
        PreviewView previewView;
        IExecutorService cameraExecutor;
        CameraSelector cameraSelector = null;
        ProcessCameraProvider cameraProvider;
        ICamera camera;
        ImageCapture? imageCapture;
        ImageCallBack imageCallback;

        public NativePlatformCameraPreviewView CreateNativeView()
        {
            imageCallback = new ImageCallBack(CameraView);
            previewView = new PreviewView(Context.Context);
            cameraExecutor = Executors.NewSingleThreadExecutor();

            return previewView;
        }

        public void Connect()
        {
            var cameraProviderFuture = ProcessCameraProvider.GetInstance(Context.Context);

            cameraProviderFuture.AddListener(new Java.Lang.Runnable(() =>
            {
                // Used to bind the lifecycle of cameras to the lifecycle owner
                cameraProvider = (ProcessCameraProvider)cameraProviderFuture.Get();

                // Preview
                cameraPreview = new AndroidX.Camera.Core.Preview.Builder().Build();
                cameraPreview.SetSurfaceProvider(previewView.SurfaceProvider);

                // Frame by frame analyze
                imageAnalyzer = new ImageAnalysis.Builder()
                    .SetDefaultResolution(new Android.Util.Size(640, 480))
                    .SetBackpressureStrategy(ImageAnalysis.StrategyKeepOnlyLatest)
                    .Build();

                imageAnalyzer.SetAnalyzer(cameraExecutor, new FrameAnalyzer((buffer, size) =>
                    FrameReady?.Invoke(this, new CameraFrameBufferEventArgs(new PixelBufferHolder { Data = buffer, Size = size }))));

                imageCapture = new ImageCapture.Builder().SetCaptureMode(ImageCapture.CaptureModeMaximizeQuality).Build();
                imageCapture.FlashMode = ImageCapture.FlashModeOff;

                UpdateCamera();

            }), ContextCompat.GetMainExecutor(Context.Context)); //GetMainExecutor: returns an Executor that runs on the main thread.
        }

        public void Disconnect()
        { }

        public void UpdateCamera()
        {
            if (cameraProvider != null)
            {
                // Unbind use cases before rebinding
                cameraProvider.UnbindAll();

                var cameraLocation = CameraLocation;

                // Select back camera as a default, or front camera otherwise
                if (cameraLocation == CameraLocation.Rear && cameraProvider.HasCamera(CameraSelector.DefaultBackCamera))
                    cameraSelector = CameraSelector.DefaultBackCamera;
                else if (cameraLocation == CameraLocation.Front && cameraProvider.HasCamera(CameraSelector.DefaultFrontCamera))
                    cameraSelector = CameraSelector.DefaultFrontCamera;
                else
                    cameraSelector = CameraSelector.DefaultBackCamera;

                if (cameraSelector == null)
                    throw new System.Exception("Camera not found");

                // The Context here SHOULD be something that's a lifecycle owner
                if (Context.Context is AndroidX.Lifecycle.ILifecycleOwner lifecycleOwner)
                {
                    camera = cameraProvider.BindToLifecycle(lifecycleOwner, cameraSelector, cameraPreview, imageAnalyzer, imageCapture);
                }
                else if (Microsoft.Maui.ApplicationModel.Platform.CurrentActivity is AndroidX.Lifecycle.ILifecycleOwner maLifecycleOwner)
                {
                    // if not, this should be sufficient as a fallback
                    camera = cameraProvider.BindToLifecycle(maLifecycleOwner, cameraSelector, cameraPreview, imageAnalyzer, imageCapture);
                }
            }
        }

        protected virtual partial void PlatformTakePicture()
        {
            imageCapture?.TakePicture(cameraExecutor, imageCallback);
        }

        public void UpdateTorch(bool on)
        {
            camera?.CameraControl?.EnableTorch(on);
        }

        public void Focus(Point point)
        {
            //TODO: Implement
        }

        public void AutoFocus()
        {
            //TODO: Implement
        }

        public void Dispose()
        {
            cameraExecutor?.Shutdown();
            cameraExecutor?.Dispose();
        }
    }
}
