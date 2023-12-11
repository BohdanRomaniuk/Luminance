using Luminance.Client.Pages;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Luminance.Client.ViewModels.Mapping
{
    public class MappingResultViewModel : BaseViewModel
    {
        private ICommand? _сreateNewMappingCommand;

        public ICommand CreateNewMappingCommand => _сreateNewMappingCommand ??= new Command(async() => await CreateNewMapping());


        public MappingResultViewModel()
        {
        }

        private async Task CreateNewMapping()
        {
            await ShowViewModelAsync<CreateMappingViewModel>();
        }
    }
}
