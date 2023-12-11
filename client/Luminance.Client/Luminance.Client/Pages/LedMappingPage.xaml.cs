using Camera.MAUI;
using Luminance.Client.ViewModels;

namespace Luminance.Client.Pages;

public partial class LedMappingPage : ContentPage
{
    public LedMappingPage()
    {
        InitializeComponent();
        BindingContext = new LedMappingViewModel(camera);
    }

    private void SwitchCamera(object sender, EventArgs e)
    {
        var current = camera.Camera.Position;
        camera.Camera = camera.Cameras.FirstOrDefault(x => x.Position != current);
    }

    private void Torch(object sender, EventArgs e)
    {
        camera.TorchEnabled = !camera.TorchEnabled;
    }

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