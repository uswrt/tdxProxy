using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TdxProxyLib;

namespace TdxCmd
{
    class Program
    {
        static void Main(string[] args)
        {
            TdxClientClass tdxClientClass=new TdxClientClass();
            tdxClientClass.Tdx_Init();
            string err="";
            tdxClientClass.Tdx_Login("123.125.24.61", 7709, "6.01","67057559", "326579", "", ref err);
            Console.WriteLine(err);
            Console.ReadLine();
            tdxClientClass.Tdx_Exit();

        }
    }
}
