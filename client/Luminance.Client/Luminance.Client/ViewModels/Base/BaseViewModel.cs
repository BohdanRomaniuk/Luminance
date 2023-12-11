using Luminance.Client.Helpers;
using Microsoft.Maui.Controls;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace Luminance.Client.ViewModels
{
    public abstract class BaseViewModel : INotifyPropertyChanged
    {
        public Page ContentPage { get; set; } = null!;


        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
           => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));

        protected Task PushPageAsync(Page page) => PushPageAsync(ContentPage.Navigation, page);

        protected async Task PushPageAsync(INavigation navigation, Page page) => await navigation.PushAsync(page);

        public Task ShowViewModelAsync<T>() where T : BaseViewModel, new()
            => PushPageAsync(CreatePage<T>());

        public Task ShowViewModelAsync<T>(params object[] args) where T : BaseViewModel
            => PushPageAsync(CreatePage<T>(args));

        public static Page CreatePage<T>(params object[] args) where T : BaseViewModel
            => PageFactory.CreatePage<T>(args);
    }
}
