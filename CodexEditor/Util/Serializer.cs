using System;
using System.IO;
using Newtonsoft.Json;

namespace CodexEditor.Util
{
	public static class Serializer
	{
		public static void ToFile<T>(string path, T value, Formatting formatting = Formatting.Indented)
		{
			try
			{
				string jsonn = JsonConvert.SerializeObject(value, formatting);
				File.WriteAllText(path, jsonn);
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.Message);
				Logger.Log(MessageType.Error, $"Failed to serialize file {path}: {ex.Message}");
				throw;
			}
		}

		public static void ToFile<T>(string path, T value, Formatting formatting, JsonSerializerSettings settings)
		{
			try
			{
				string jsonn = JsonConvert.SerializeObject(value, formatting, settings);
				File.WriteAllText(path, jsonn);
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.Message);
				Logger.Log(MessageType.Error, $"Failed to serialize file {path}: {ex.Message}");
				throw;
			}
		}

		public static T FromFile<T>(string path)
		{
			try
			{
				var jsonData = File.ReadAllText(path);
				var obj = JsonConvert.DeserializeObject<T>(jsonData);
				return obj;
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.Message);
				Logger.Log(MessageType.Error, $"Failed to deserialize file {path}: {ex.Message}");
				throw; 
			}
		}

		public static T FromFile<T>(string path, JsonSerializerSettings settings)
		{
			try
			{
				var jsonData = File.ReadAllText(path);
				var obj = JsonConvert.DeserializeObject<T>(jsonData, settings);
				return obj;
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.Message);
				Logger.Log(MessageType.Error, $"Failed to deserialize file {path}: {ex.Message}");
				throw;
			}
		}
	}
}
