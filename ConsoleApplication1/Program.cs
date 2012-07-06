using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Data;
using System.Data.SqlClient;

namespace ConsoleApplication1
{
    class Program
    {
        static void Main(string[] args)
        {
            //FileStream fs = new FileStream(@"rand.dic", FileMode.Open);

            //StreamReader sr = new StreamReader(fs);

            //SqlConnection con = new SqlConnection(@"Data Source=.\SQLEXPRESS;Initial Catalog=Test;Integrated Security=True");
            //SqlCommand cmd = new SqlCommand(@"insert into rands ([ki] ,[rand1],[rand2]) VALUES (@ki,@rand1,@rand2)", con);
            //SqlParameter spki = new SqlParameter("@ki", SqlDbType.NChar);
            //SqlParameter sprand1 = new SqlParameter("@rand1", SqlDbType.NChar);
            //SqlParameter sprand2 = new SqlParameter("@rand2", SqlDbType.NChar);
            //cmd.Parameters.Add(spki);
            //cmd.Parameters.Add(sprand1);
            //cmd.Parameters.Add(sprand2);
            //con.Open();

            //while (!sr.EndOfStream)
            //{
            //    string[] col = sr.ReadLine().Split(' ');
            //    if (col.Length == 1)
            //    {
            //        spki.Value = col[0];
            //        Console.WriteLine(col[0]);
            //    }
            //    else
            //    {
            //        sprand1.Value = col[0];
            //        sprand2.Value = col[1];
            //        cmd.ExecuteNonQuery();
            //        //Console.WriteLine ("{0} {1}",col[0],col[i]);
            //    }
            //}

            FileStream f1 = new FileStream(@"r1.dic", FileMode.Open);
            FileStream f2 = new FileStream(@"r2.dic", FileMode.Open);

            StreamReader sr1 = new StreamReader(f1);
            StreamReader sr2 = new StreamReader(f2);

            SqlConnection con = new SqlConnection(@"Data Source=.\SQLEXPRESS;Initial Catalog=Test;Integrated Security=True");
            SqlCommand cmd = new SqlCommand(@"insert into randdic ([rand]) VALUES (@rand)", con);
            SqlParameter sprand = new SqlParameter("@rand", SqlDbType.NChar);
            cmd.Parameters.Add(sprand);
            con.Open();

            while (!sr1.EndOfStream)
            {
                string[] col = sr1.ReadLine().Split(' ');
                sprand.Value = col[0];
                cmd.ExecuteNonQuery();

                int cnt = Convert.ToInt32(col[1]);
                for (int i = 0; i < cnt; ++i)
                {
                    sprand.Value = sr2.ReadLine();
                    cmd.ExecuteNonQuery();
                }       
            }
        }
    }
}
