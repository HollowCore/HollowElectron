#include <node_api.h>
#include "../HollowCore/Source/HollowCore.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - N-API Convenience
//----------------------------------------------------------------------------------------------------------------------------------
void HCObjectNAPIOnLoad(napi_env env, napi_value exports) {
}

void HCObjectNAPIInstallReferenceInValue(napi_env env, napi_value value, HCRef self) {
    napi_status status = napi_generic_failure;
    if (value == NULL || self == NULL) {
        return;
    }
    
    napi_value reference_value = NULL;
    status = napi_create_int64(env, (int64_t)self, &reference_value);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create reference value from reference");
    }
    
    status = napi_set_named_property(env, value, "reference", reference_value);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to install reference value into value");
    }
}

void HCObjectNAPIReleaseReferenceInValue(napi_env env, napi_value value) {
    napi_status status = napi_generic_failure;
    if (value == NULL) {
        return;
    }
    
    napi_value reference_value = NULL;
    status = napi_get_named_property(env, value, "reference", &reference_value);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to obtain reference value from value");
    }
    
    HCRef reference = NULL;
    status = napi_get_value_int64(env, reference_value, (int64_t*)&reference);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to convert reference value to reference");
    }
    
    napi_value null_value = NULL;
    status = napi_get_null(env, &null_value);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to obtain the null value");
    }
    
    status = napi_set_named_property(env, value, "reference", null_value);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to clear reference value in value");
    }
    
    HCRelease(reference);
}

HCRef HCObjectNAPIFromValue(napi_env env, napi_value value) {
    napi_status status = napi_generic_failure;
    if (value == NULL) {
        return NULL;
    }
    
    napi_value reference_value = NULL;
    status = napi_get_named_property(env, value, "reference", &reference_value);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to obtain reference value from value");
    }
    
    HCRef reference = NULL;
    status = napi_get_value_int64(env, reference_value, (int64_t*)&reference);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to convert reference value to reference");
    }
    return reference;
}

napi_value HCObjectNAPINewValue(napi_env env, HCRef self) {
    napi_status status = napi_generic_failure;
    if (self == NULL) {
        return NULL;
    }
    
    napi_value value = NULL;
    status = napi_create_object(env, &value);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create object");
    }

    HCObjectNAPIInstallReferenceInValue(env, value, self);
    
    return value;
}

napi_value HCObjectNAPINewBoolean(napi_env env, HCBoolean boolean) {
    napi_status status = napi_generic_failure;
    napi_value boolean_value = NULL;
    status = napi_get_boolean(env, boolean, &boolean_value);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create boolean value");
    }
    return boolean_value;
}

napi_value HCObjectNAPINewInteger(napi_env env, HCInteger integer) {
    napi_status status = napi_generic_failure;
    napi_value integer_value = NULL;
    status = napi_create_int64(env, integer, &integer_value);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create integer value");
    }
    return integer_value;
}

napi_value HCObjectNAPINewReal(napi_env env, HCReal real) {
    napi_status status = napi_generic_failure;
    napi_value real_value = NULL;
    status = napi_create_double(env, real, &real_value);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create integer value");
    }
    return real_value;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - HCPath
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathNAPIOnLoad(napi_env env) {
}

void HCPathNAPIInstallReferenceInValue(napi_env env, napi_value value, HCPathRef self) {
    HCPathNAPIInstallReferenceInValue(env, value, self);
}

void HCPathNAPIReleaseReferenceInValue(napi_env env, napi_value value) {
    HCPathNAPIReleaseReferenceInValue(env, value);
}

HCPathRef HCPathNAPIFromValue(napi_env env, napi_value value) {
    return HCObjectNAPIFromValue(env, value);
}

napi_value HCPathNAPINewValue(napi_env env, HCPathRef self) {
    return HCObjectNAPINewValue(env, self);
}

napi_value HCPathNAPICreate(napi_env env, napi_value svg_path_data_value) {
    napi_status status = napi_generic_failure;
    
    size_t svg_path_data_length = 0;
    status = napi_get_value_string_utf8(env, svg_path_data_value, NULL, sizeof(svg_path_data_length), &svg_path_data_length);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to get length of string argument");
    }
    char svg_path_data[svg_path_data_length];
    size_t svg_path_data_copied_length = 0;
    status = napi_get_value_string_utf8(env, svg_path_data_value, svg_path_data, svg_path_data_length, &svg_path_data_copied_length);
    if (status != napi_ok) {//} || svg_path_data_length != svg_path_data_copied_length) {
        napi_throw_error(env, NULL, "Unable to copy string argument");
    }
    
    HCPathRef self = HCPathCreate(svg_path_data);
    return HCPathNAPINewValue(env, self);
}

napi_value HCPathNAPIElementCount(napi_env env, napi_value value) {
    HCPathRef self = HCPathNAPIFromValue(env, value);
    HCInteger elementCount = HCPathElementCount(self);
    return HCObjectNAPINewInteger(env, elementCount);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Sample Functions
//----------------------------------------------------------------------------------------------------------------------------------
napi_value createPath(napi_env env, napi_callback_info info) {
    napi_status status = napi_generic_failure;
    
    size_t argc = 1;
    napi_value argv[1];
    status = napi_get_cb_info(env, info, &argc, argv, /*thisArg*/NULL, /*context*/NULL);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to get function arguments");
    }
    
    return HCPathNAPICreate(env, argv[0]);
}

napi_value pathElementCount(napi_env env, napi_callback_info info) {
    napi_status status = napi_generic_failure;
    
    size_t argc = 1;
    napi_value argv[1];
    status = napi_get_cb_info(env, info, &argc, argv, /*thisArg*/NULL, /*context*/NULL);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to get function arguments");
    }
    
    return HCPathNAPIElementCount(env, argv[0]);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - N-API Module Registration
//----------------------------------------------------------------------------------------------------------------------------------

napi_value Init(napi_env env, napi_value exports) {
    napi_status status = napi_generic_failure;
    
    napi_value hcPathCreateFunction;
    status = napi_create_function(env, "createPath", NAPI_AUTO_LENGTH, createPath, NULL, &hcPathCreateFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create native function");
    }
    status = napi_set_named_property(env, exports, "createPath", hcPathCreateFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to add native function to exports");
    }
    napi_value hcPathElementCountFunction;
    status = napi_create_function(env, "pathElementCount", NAPI_AUTO_LENGTH, pathElementCount, NULL, &hcPathElementCountFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create native function");
    }
    status = napi_set_named_property(env, exports, "pathElementCount", hcPathElementCountFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to add native function to exports");
    }

    return exports;
}

NAPI_MODULE_INIT() {
    return Init(env, exports);
}
