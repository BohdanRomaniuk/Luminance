using System.Diagnostics;
using System.Runtime.CompilerServices;

namespace Luminance.Helpers
{
    public static class ExceptionHelper
    {
        public static void Report(this Exception exception, [CallerMemberName] string? methodName = null)
        {
            if (exception == null)
            {
                return;
            }

            Debug.WriteLine($"{methodName} : {exception.Message}");
            Debug.WriteLine(exception.StackTrace);
        }
    }
}
