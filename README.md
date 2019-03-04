# max-py: Python externals for MaxMSP
[![Build Status](https://travis-ci.org/njazz/max-py.svg?branch=master)](https://travis-ci.org/njazz/max-py)
[![Build status](https://ci.appveyor.com/api/projects/status/lu577mn5euj54tra?svg=true)](https://ci.appveyor.com/project/njazz/max-py)
[![FOSSA Status](https://app.fossa.io/api/projects/git%2Bgithub.com%2Fnjazz%2Fmax-py.svg?type=shield)](https://app.fossa.io/projects/git%2Bgithub.com%2Fnjazz%2Fmax-py?ref=badge_shield)



Embedding Python 2 / 3 in MaxMSP with pybind11

Objects:

**[py.eval]** converts Max message to string and passes it to Python eval(). Outputs Python object converted to Max message or single atom.

**[py.exec]** converts Max message to string and runs it with Python exec().

**[py.func]** binds object to specified Python function ([py.func (class.)function_name] or [py.func module (class.)function_name]). Accepts Max message, outputs Python object converted to Max message or single atom.

**[py.module]** allows to call functions with [(class.)function_name args...] messages to object [py.module module_name]. Otherwise work same as [py.func]

**[py.callback]** works with callback_name as argument. Adds function 'callback_name()' to the embedded 'max' module. This new function can be called directly from Python code. When it is called from Python code, outputs its arguments through its outlet. 'callback_name' should be unique for each [py.callback] box.

**[py.receive]** works the same way as [py.callback] but allows multiple instances with the same callback_name.

## Currently supported Python types:

- int, float, str -> Max atom

- list -> Max list

- any other object -> Max symbol containing str(object) value

Embedded 'max' module includes:

- **max.post()**, **max.error()** : Max console messages

EXPERIMENTAL/INCOMPLETE object functions

- **max.new_object()**

- **object.message()**

- **object.inlet_count()**

- **object.outlet_count()**

- **object.class_name()**

- **object.method_names()**

Current version: 0.0.1
