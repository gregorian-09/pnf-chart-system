using System;
using System.Runtime.InteropServices;

namespace PnF
{
    /// <summary>
    /// Version helpers for the native library.
    /// </summary>
    public static class Version
    {
        public static string String
        {
            get
            {
                var ptr = NativeMethods.pnf_version_string();
                if (ptr == IntPtr.Zero) return "unknown";
                var result = Marshal.PtrToStringAnsi(ptr) ?? "unknown";
                NativeMethods.pnf_free_string(ptr);
                return result;
            }
        }

        public static int Major => NativeMethods.pnf_version_major();

        public static int Minor => NativeMethods.pnf_version_minor();

        public static int Patch => NativeMethods.pnf_version_patch();

        public static string Full => $"{Major}.{Minor}.{Patch}";
    }
}
