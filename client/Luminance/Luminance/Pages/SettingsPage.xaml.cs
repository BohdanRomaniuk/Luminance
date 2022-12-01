using Luminance.ViewModels;

namespace Luminance.Pages;

public partial class SettingsPage : ContentPage
{
    public SettingsPage()
    {
        InitializeComponent();
        BindingContext = new SettingsViewModel();
    }
}