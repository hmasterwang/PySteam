/**************************************************************************
 * Copyright (c) 2015 Afa.L Cheng <alpha@afa.moe>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ***************************************************************************/

#include <Python.h>
#include "global.h"

#include "achievements.h"

int initSteam();
void shutdownSteam();

static PyObject* pysteam_init(PyObject *self, PyObject *args)
{
    UNUSED(self);
    UNUSED(args);
    if (initSteam())
        exit(-1);

    Achievements::init();

    SteamAPI_RunCallbacks();

    return Py_True;
}

static PyObject* pysteam_shutdown(PyObject *self, PyObject *args)
{
    UNUSED(self);
    UNUSED(args);

    Achievements::finalize();
    SteamAPI_RunCallbacks();

    shutdownSteam();
    return Py_None;
}

static PyObject* pysteam_setAchievement(PyObject *self, PyObject *args)
{
    UNUSED(self);
    PyObject *argAchName = 0;

    if (!PyArg_UnpackTuple(args, "setAchievement", 1, 1, &argAchName))
        return Py_None;

    PyObject *achNameStr = PyObject_Str(argAchName);
    if (!achNameStr)
        return Py_None;

    const char *achName = PyString_AsString(achNameStr);
    if (!achName)
        return Py_None;

    bool result = Achievements::instance()->setAchievement(achName);
    SteamAPI_RunCallbacks();
    return result ? Py_True : Py_False;
}

static PyObject* pysteam_runCallbacks(PyObject *self, PyObject *args)
{
    UNUSED(self);
    UNUSED(args);
    SteamAPI_RunCallbacks();
    return Py_None;
}

static PyMethodDef PySteamMethods[] =
{
    { "initialize",     pysteam_init,           METH_VARARGS, "Init Steam"      },
    { "shutdown",       pysteam_shutdown,       METH_VARARGS, "Shutdown Steam"  },
    { "setAchievement", pysteam_setAchievement, METH_VARARGS, "Set Achievement" },
    { "runCallbacks",   pysteam_runCallbacks,   METH_VARARGS, "Run callbacks"  },
    { NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC initpysteam(void)
{
    (void)Py_InitModule("pysteam", PySteamMethods);
}
