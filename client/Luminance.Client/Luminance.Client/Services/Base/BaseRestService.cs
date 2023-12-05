using Luminance.Client.Helpers;
using Luminance.Client.Services.Interfaces;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Net;
using System.Text;

namespace Luminance.Client.Services
{
    public class BaseRestService : IBaseRestClient
    {
        protected static HttpClient HttpClient { get; private set; } = null!;

        public BaseRestService()
        {
            HttpClient = new HttpClient()
            {
                Timeout = TimeSpan.FromSeconds(30),
            };
        }

        public void SetBaseAddress(IPAddress ip)
            => HttpClient.BaseAddress ??= new Uri($"http://{ip}/api/");

        protected async Task<T> GetAsync<T>(string requestUri, string? parameters = null)
        {
            try
            {
                var uri = string.IsNullOrWhiteSpace(parameters) ? requestUri : $"{requestUri}?{parameters}";

                return await GetResponseAsync<T>(await HttpClient.GetAsync(uri));
            }
            catch (Exception ex)
            {
                ex.Report();

                return default;
            }
        }

        protected async Task<T> PostAsync<T>(string requestUri, JObject body)
        {
            try
            {
                var json = JsonConvert.SerializeObject(body);
                var data = new StringContent(json, Encoding.UTF8, "application/json");

                return await GetResponseAsync<T>(await HttpClient.PostAsync(requestUri, data));
            }
            catch (Exception ex)
            {
                ex.Report();

                return default;
            }
        }

        protected async Task<T> GetResponseAsync<T>(HttpResponseMessage response)
        {
            try
            {
                var content = await response.Content.ReadAsStringAsync();

                return JsonConvert.DeserializeObject<T>(content);
            }
            catch (Exception ex)
            {
                ex.Report();

                return default;
            }
        }
    }
}
