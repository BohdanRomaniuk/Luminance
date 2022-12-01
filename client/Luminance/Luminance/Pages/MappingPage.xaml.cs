using Luminance.CameraView;

namespace Luminance.Pages;

public partial class MappingPage : ContentPage
{
    public MappingPage()
    {
        InitializeComponent();
    }

    private void FrameReady(object sender, CameraView.CameraFrameBufferEventArgs e)
    {

    }

    private void SwitchCamera(object sender, EventArgs e)
    {
        camera.CameraLocation = camera.CameraLocation == CameraLocation.Rear ? CameraLocation.Front : CameraLocation.Rear;
    }

    private void Torch(object sender, EventArgs e)
    {
        camera.IsTorchOn = !camera.IsTorchOn;
    }

    private void StartMapping(object sender, EventArgs e)
    {
        camera.Shutter();
    }

    private void MediaCaptured(object sender, MediaCapturedEventArgs e)
    {
        var image = ImageSource.FromStream(() => e.Media);
        MainThread.BeginInvokeOnMainThread(() => imgView.Source = image);
    }
}