using Camera.MAUI;

namespace Luminance.Client.Pages;

public partial class LedMappingPage : ContentPage
{
	public LedMappingPage()
	{
		InitializeComponent();
        camera.CamerasLoaded += CameraView_CamerasLoaded;
    }

    private void CameraView_CamerasLoaded(object sender, EventArgs e)
    {
        if (camera.NumCamerasDetected > 0)
        {
            if (camera.NumMicrophonesDetected > 0)
            {
                camera.Microphone = camera.Microphones.First();
            }
                
            camera.Camera = camera.Cameras.First();
            MainThread.BeginInvokeOnMainThread(async () =>
            {
                if (await camera.StartCameraAsync() == CameraResult.Success)
                {
                }
            });
        }
    }

    private void SwitchCamera(object sender, EventArgs e)
    {
        //camera.CameraLocation = camera.CameraLocation == CameraLocation.Rear ? CameraLocation.Front : CameraLocation.Rear;
    }



    private void Torch(object sender, EventArgs e)
        => camera.TorchEnabled = !camera.TorchEnabled;

    private async void StartMapping(object sender, EventArgs e)
    {
        var stream = await camera.TakePhotoAsync();
        if (stream != null)
        {
            var result = ImageSource.FromStream(() => stream);
            imgView.Source = result;
        }
    }
}