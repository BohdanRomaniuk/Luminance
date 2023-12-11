using Luminance.Client.ViewModels.Mapping;

namespace Luminance.Client.Pages;

public partial class MappingResultPage : ContentPage
{
    public MappingResultPage()
    {
        InitializeComponent();
        BindingContext = new MappingResultViewModel()
        {
            ContentPage = this,
        };
    }
}