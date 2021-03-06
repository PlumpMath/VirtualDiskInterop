#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace VirtualDiskInterop
{
	public ref class Helpers
	{
	public:
		static String^ GetSystemErrorMessage(unsigned int errorCode)
		{
			WCHAR buffer[500];
			memset(buffer, 0, 500 * sizeof(WCHAR));
			FormatMessageW(
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				errorCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
				buffer,
				500,
				NULL);
			return gcnew String(buffer);
		}

	internal:
		// ToGUID and FromGUID from https://msdn.microsoft.com/en-us/library/wb8scw8f.aspx
		static Guid FromGUID(_GUID guid) {
			return Guid(guid.Data1, guid.Data2, guid.Data3,
				guid.Data4[0], guid.Data4[1],
				guid.Data4[2], guid.Data4[3],
				guid.Data4[4], guid.Data4[5],
				guid.Data4[6], guid.Data4[7]);
		}

		static _GUID ToGUID(Guid guid) {
			array<Byte>^ guidData = guid.ToByteArray();
			pin_ptr<Byte> data = &(guidData[0]);
			return *(_GUID *)data;
		}


		static String^ ArrayToMultiSz(array<String^>^ data)
		{
			String^ result = String::Join(L"\0", data);
			result = String::Concat(result, L"\0\0");
			return result;
		}
		static array<String^>^ MultiSzToArray(WCHAR* rawData)
		{
			List<String^>^ result = gcnew List<String^>();

			int offset = 0;
			String^ data = gcnew String(rawData);

			while (!String::IsNullOrEmpty(data))
			{
				result->Add(data);
				offset += (data->Length + 1) * 2;
				data = gcnew String(rawData + offset);
			}

			return result->ToArray();
		}

		static LPWSTR AllocString(String^ value)
		{
			LPWSTR result = NULL;
			if (!String::IsNullOrEmpty(value))
			{
				int cb = (value->Length + 1) * sizeof(Char);
				result = (LPWSTR)LocalAlloc(LPTR, cb);
				pin_ptr<const WCHAR> pszNative = PtrToStringChars(value);
				memcpy_s(result, cb, pszNative, cb);
			}
			return result;
		}

	};
}