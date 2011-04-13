#include <ecto/module.hpp>
#include <ecto/ecto.hpp>

#include <boost/python.hpp>
#include <boost/python/raw_function.hpp>
#include <ecto/python/std_map_indexing_suite.hpp>

#include <ecto/python/raw_constructor.hpp>
#include <ecto/python/repr.hpp>

namespace bp = boost::python;

namespace ecto
{
  namespace py
  {

    struct modwrap : module, bp::wrapper<module>
    {

      void Process()
      {
	if(bp::override process = this->get_override("Process"))
	  process();
	else
	  throw std::logic_error("Process is not overridden it seems");
      }

      void Config()
      {
	if (bp::override config = this->get_override("Config"))
	  config();
	else
	  throw std::logic_error("Config is not overridden it seems");
      }

      std::string name()
      {
        bp::reference_existing_object::apply<modwrap*>::type converter;
        PyObject* obj = converter(this);
        bp::object real_obj = bp::object(bp::handle<>(obj));
        bp::object n = real_obj.attr("__class__").attr("__name__");
        std::string nm = bp::extract<std::string>(n);
        return nm;
      }

      static std::string doc(modwrap* mod)
      {
        bp::reference_existing_object::apply<modwrap*>::type converter;
        PyObject* obj = converter(mod);
        bp::object real_obj = bp::object(bp::handle<>(obj));
        bp::object n = real_obj.attr("__class__").attr("__doc__");
        std::string nm = bp::extract<std::string>(n);
        return nm;
      }
    };

    void wrapModule()
    {
      bp::class_<module, boost::shared_ptr<module>, boost::noncopyable>("module_cpp");

      bp::class_<modwrap, boost::shared_ptr<modwrap>, boost::noncopyable>("module_base"/*, bp::no_init*/)
	.def("connect", &module::connect)
	.def("Process", bp::pure_virtual(&module::Process))
	.def("Config", bp::pure_virtual(&module::Config))
	.def_readonly("inputs", &module::inputs)
	.def_readonly("outputs", &module::outputs)
	.def_readonly("params", &module::params)
	.def("Name", &module::name)
	.def("Doc", &modwrap::doc)
	;
    }

  }
}

