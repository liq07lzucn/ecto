/*
 * Copyright (c) 2011, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <ecto/ecto.hpp>
#include <iostream>

namespace hello_ecto
{

  using ecto::tendrils;

  /* BOILER_PLATE_MODULE
   struct MyModule
   {
   static void declare_params(tendrils& params);
   static void declare_io(const tendrils& params, tendrils& in, tendrils& out);
   void configure(tendrils& params, tendrils& inputs, tendrils& outputs);
   int process(const tendrils& in, tendrils& out);
   void destroy();
   };
   */

  struct Printer
  {
    static void declare_params(tendrils& params)
    {
      params.declare<std::string> ("str", "The default string to print", "hello");
    }

    static void declare_io(const tendrils& parms, tendrils& in, tendrils& out)
    {
      in.declare<std::string> ("str", "The string to print.", parms.get<std::string> ("str"));
    }

    Printer() :
      str_()
    {
    }

    void configure(tendrils& params, tendrils& inputs, tendrils& outputs)
    {
      str_ = params.get<std::string> ("str");
    }

    int process(const tendrils& in, tendrils& /*out*/)
    {
      std::cout << in.get<std::string> ("str") << std::endl;
      return 0;
    }
    std::string str_;
  };

  struct Reader
  {
    static void declare_io(const tendrils& parms, tendrils& in, tendrils& out)
    {
      out.declare<std::string> ("output", "Output from standard in");
    }

    int process(const tendrils& in, tendrils& out)
    {
      std::string s;
      std::cin >> s;
      out.get<std::string> ("output") = s;
      return ecto::OK;
    }
  };

}

BOOST_PYTHON_MODULE(hello_ecto)
{
  using namespace hello_ecto;
  ecto::wrap<Printer>("Printer", "Prints a string input to standard output.");
  ecto::wrap<Reader>("Reader", "Reads input from standard input.");
}
