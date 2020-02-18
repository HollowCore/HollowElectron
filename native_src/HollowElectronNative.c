#include <node_api.h>
#include "../HollowCore/Source/HollowCore.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - N-API Convenience
//----------------------------------------------------------------------------------------------------------------------------------
#define HCObjectNAPIReferenceProperty "_reference"

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
    
    status = napi_set_named_property(env, value, HCObjectNAPIReferenceProperty, reference_value);
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
    status = napi_get_named_property(env, value, HCObjectNAPIReferenceProperty, &reference_value);
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
    
    status = napi_set_named_property(env, value, HCObjectNAPIReferenceProperty, null_value);
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
    status = napi_get_named_property(env, value, HCObjectNAPIReferenceProperty, &reference_value);
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

HCBoolean HCBooleanNAPIFromValue(napi_env env, napi_value boolean_value) {
    napi_status status = napi_generic_failure;
    HCBoolean boolean = false;
    status = napi_get_value_bool(env, boolean_value, (bool*)&boolean);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to extract boolean from value");
    }
    return boolean;
}

napi_value HCBooleanNAPINewValue(napi_env env, HCBoolean boolean) {
    napi_status status = napi_generic_failure;
    napi_value boolean_value = NULL;
    status = napi_get_boolean(env, boolean, &boolean_value);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create boolean value");
    }
    return boolean_value;
}

HCInteger HCIntegerNAPIFromValue(napi_env env, napi_value integer_value) {
    napi_status status = napi_generic_failure;
    HCInteger integer = 0;
    status = napi_get_value_int64(env, integer_value, (int64_t*)&integer);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to extract integer from value");
    }
    return integer;
}

napi_value HCIntegerNAPINewValue(napi_env env, HCInteger integer) {
    napi_status status = napi_generic_failure;
    napi_value integer_value = NULL;
    status = napi_create_int64(env, integer, &integer_value);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create integer value");
    }
    return integer_value;
}

HCReal HCRealNAPIFromValue(napi_env env, napi_value real_value) {
    napi_status status = napi_generic_failure;
    HCReal real = 0.0;
    status = napi_get_value_double(env, real_value, (double*)&real);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to extract real from value");
    }
    return real;
}

napi_value HCRealNAPINewValue(napi_env env, HCReal real) {
    napi_status status = napi_generic_failure;
    napi_value real_value = NULL;
    status = napi_create_double(env, real, &real_value);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create real value");
    }
    return real_value;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Geometry
//----------------------------------------------------------------------------------------------------------------------------------
napi_value HCPointNAPINewValue(napi_env env, HCPoint self) {
    napi_status status = napi_generic_failure;
    
    napi_value value = NULL;
    status = napi_create_object(env, &value);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create object");
    }
    status = napi_set_named_property(env, value, "x", HCRealNAPINewValue(env, self.x));
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to set x property in point");
    }
    status = napi_set_named_property(env, value, "y", HCRealNAPINewValue(env, self.y));
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to set y property in point");
    }

    // TODO: Want reference? Use HCData?
//    HCObjectNAPIInstallReferenceInValue(env, value, self);
    
    return value;
}

napi_value HCSizeNAPINewValue(napi_env env, HCSize self) {
    napi_status status = napi_generic_failure;
    
    napi_value value = NULL;
    status = napi_create_object(env, &value);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create object");
    }
    status = napi_set_named_property(env, value, "width", HCRealNAPINewValue(env, self.width));
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to set width property in size");
    }
    status = napi_set_named_property(env, value, "height", HCRealNAPINewValue(env, self.height));
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to set height property in size");
    }

    // TODO: Want reference? Use HCData?
//    HCObjectNAPIInstallReferenceInValue(env, value, self);
    
    return value;
}

napi_value HCRectangleNAPINewValue(napi_env env, HCRectangle self) {
    napi_status status = napi_generic_failure;
    
    napi_value value = NULL;
    status = napi_create_object(env, &value);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create object");
    }
    status = napi_set_named_property(env, value, "origin", HCPointNAPINewValue(env, self.origin));
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to set origin property in rectangle");
    }
    status = napi_set_named_property(env, value, "size", HCSizeNAPINewValue(env, self.size));
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to set size property in rectangle");
    }

    // TODO: Want reference? Use HCData?
//    HCObjectNAPIInstallReferenceInValue(env, value, self);
    
    return value;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - HCPath
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathNAPIOnLoad(napi_env env) {
}

void HCPathNAPIInstallReferenceInValue(napi_env env, napi_value value, HCPathRef self) {
    HCObjectNAPIInstallReferenceInValue(env, value, self);
}

void HCPathNAPIReleaseReferenceInValue(napi_env env, napi_value value) {
    HCObjectNAPIReleaseReferenceInValue(env, value);
}

HCPathRef HCPathNAPIFromValue(napi_env env, napi_value value) {
    return HCObjectNAPIFromValue(env, value);
}

napi_value HCPathNAPINewValue(napi_env env, HCPathRef self) {
    return HCObjectNAPINewValue(env, self);
}

void HCPathNAPIInit(napi_env env, napi_value value, napi_value svg_path_data_value) {
    napi_status status = napi_generic_failure;
    
    size_t svg_path_data_length = 0;
    status = napi_get_value_string_utf8(env, svg_path_data_value, NULL, sizeof(svg_path_data_length), &svg_path_data_length);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to get length of string argument");
    }
    char svg_path_data[svg_path_data_length + 1];
    size_t svg_path_data_copied_length = 0;
    status = napi_get_value_string_utf8(env, svg_path_data_value, svg_path_data, sizeof(svg_path_data), &svg_path_data_copied_length);
    if (status != napi_ok || svg_path_data_length != svg_path_data_copied_length) {
        napi_throw_error(env, NULL, "Unable to copy string argument");
    }
    
    HCPathRef self = HCPathCreate(svg_path_data);
    HCPathNAPIInstallReferenceInValue(env, value, self);
}

napi_value HCPathNAPIElementCount(napi_env env, napi_value value) {
    HCPathRef self = HCPathNAPIFromValue(env, value);
    HCInteger elementCount = HCPathElementCount(self);
    return HCIntegerNAPINewValue(env, elementCount);
}

napi_value HCPathNAPICurrentPoint(napi_env env, napi_value value) {
    HCPathRef self = HCPathNAPIFromValue(env, value);
    HCPoint currentPoint = HCPathCurrentPoint(self);
    return HCPointNAPINewValue(env, currentPoint);
}

napi_value HCPathNAPIBounds(napi_env env, napi_value value) {
    HCPathRef self = HCPathNAPIFromValue(env, value);
    HCRectangle bounds = HCPathBounds(self);
    return HCRectangleNAPINewValue(env, bounds);
}

void HCPathNAPIMove(napi_env env, napi_value value, napi_value x_value, napi_value y_value) {
    HCPathRef self = HCPathNAPIFromValue(env, value);
    HCReal x = HCRealNAPIFromValue(env, x_value);
    HCReal y = HCRealNAPIFromValue(env, y_value);
    HCPathMoveToPoint(self, x, y);
}

void HCPathNAPIAddLine(napi_env env, napi_value value, napi_value x_value, napi_value y_value) {
    HCPathRef self = HCPathNAPIFromValue(env, value);
    HCReal x = HCRealNAPIFromValue(env, x_value);
    HCReal y = HCRealNAPIFromValue(env, y_value);
    HCPathAddLine(self, x, y);
}

void HCPathNAPIAddQuadraticCurve(napi_env env, napi_value value, napi_value cx_value, napi_value cy_value, napi_value x_value, napi_value y_value) {
    HCPathRef self = HCPathNAPIFromValue(env, value);
    HCReal cx = HCRealNAPIFromValue(env, cx_value);
    HCReal cy = HCRealNAPIFromValue(env, cy_value);
    HCReal x = HCRealNAPIFromValue(env, x_value);
    HCReal y = HCRealNAPIFromValue(env, y_value);
    HCPathAddQuadraticCurve(self, cx, cy, x, y);
}

void HCPathNAPIAddCubicCurve(napi_env env, napi_value value, napi_value cx0_value, napi_value cy0_value, napi_value cx1_value, napi_value cy1_value, napi_value x_value, napi_value y_value) {
    HCPathRef self = HCPathNAPIFromValue(env, value);
    HCReal cx0 = HCRealNAPIFromValue(env, cx0_value);
    HCReal cy0 = HCRealNAPIFromValue(env, cy0_value);
    HCReal cx1 = HCRealNAPIFromValue(env, cx1_value);
    HCReal cy1 = HCRealNAPIFromValue(env, cy1_value);
    HCReal x = HCRealNAPIFromValue(env, x_value);
    HCReal y = HCRealNAPIFromValue(env, y_value);
    HCPathAddCubicCurve(self, cx0, cy0, cx1, cy1, x, y);
}

void HCPathNAPIClose(napi_env env, napi_value value) {
    HCPathRef self = HCPathNAPIFromValue(env, value);
    HCPathCloseSubpath(self);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Function Argument Unpacking
//----------------------------------------------------------------------------------------------------------------------------------
napi_value pathInit(napi_env env, napi_callback_info info) {
    napi_status status = napi_generic_failure;
    
    size_t argc = 1;
    napi_value argv[1];
    napi_value value;
    status = napi_get_cb_info(env, info, &argc, argv, &value, NULL);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to get function arguments");
    }
    
    HCPathNAPIInit(env, value, argv[0]);
    
    return NULL;
}

napi_value pathElementCount(napi_env env, napi_callback_info info) {
    napi_status status = napi_generic_failure;
    
    size_t argc = 0;
    napi_value value;
    status = napi_get_cb_info(env, info, &argc, NULL, &value, NULL);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to get function arguments");
    }
    
    return HCPathNAPIElementCount(env, value);
}

napi_value pathCurrentPoint(napi_env env, napi_callback_info info) {
    napi_status status = napi_generic_failure;
    
    size_t argc = 0;
    napi_value value;
    status = napi_get_cb_info(env, info, &argc, NULL, &value, NULL);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to get function arguments");
    }
    
    return HCPathNAPICurrentPoint(env, value);
}

napi_value pathBounds(napi_env env, napi_callback_info info) {
    napi_status status = napi_generic_failure;
    
    size_t argc = 0;
    napi_value value;
    status = napi_get_cb_info(env, info, &argc, NULL, &value, NULL);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to get function arguments");
    }
    
    return HCPathNAPIBounds(env, value);
}

napi_value pathMove(napi_env env, napi_callback_info info) {
    napi_status status = napi_generic_failure;
    
    size_t argc = 2;
    napi_value argv[2];
    napi_value value;
    status = napi_get_cb_info(env, info, &argc, argv, &value, NULL);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to get function arguments");
    }
    
    HCPathNAPIMove(env, value, argv[0], argv[1]);
    return NULL;
}

napi_value pathAddLine(napi_env env, napi_callback_info info) {
    napi_status status = napi_generic_failure;
    
    size_t argc = 2;
    napi_value argv[2];
    napi_value value;
    status = napi_get_cb_info(env, info, &argc, argv, &value, NULL);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to get function arguments");
    }
    
    HCPathNAPIAddLine(env, value, argv[0], argv[1]);
    return NULL;
}

napi_value pathAddQuadraticCurve(napi_env env, napi_callback_info info) {
    napi_status status = napi_generic_failure;
    
    size_t argc = 4;
    napi_value argv[4];
    napi_value value;
    status = napi_get_cb_info(env, info, &argc, argv, &value, NULL);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to get function arguments");
    }
    
    HCPathNAPIAddQuadraticCurve(env, value, argv[0], argv[1], argv[2], argv[3]);
    return NULL;
}

napi_value pathAddCubicCurve(napi_env env, napi_callback_info info) {
    napi_status status = napi_generic_failure;
    
    size_t argc = 6;
    napi_value argv[6];
    napi_value value;
    status = napi_get_cb_info(env, info, &argc, argv, &value, NULL);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to get function arguments");
    }
    
    HCPathNAPIAddCubicCurve(env, value, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);
    return NULL;
}

napi_value pathClose(napi_env env, napi_callback_info info) {
    napi_status status = napi_generic_failure;
    
    size_t argc = 0;
    napi_value value;
    status = napi_get_cb_info(env, info, &argc, NULL, &value, NULL);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to get function arguments");
    }
    
    HCPathNAPIClose(env, value);
    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - N-API Module Registration
//----------------------------------------------------------------------------------------------------------------------------------

napi_value Init(napi_env env, napi_value exports) {
    napi_status status = napi_generic_failure;
    
    napi_value pathInitFunction;
    status = napi_create_function(env, "pathInit", NAPI_AUTO_LENGTH, pathInit, NULL, &pathInitFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create native function");
    }
    status = napi_set_named_property(env, exports, "pathInit", pathInitFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to add native function to exports");
    }
    
    napi_value pathElementCountFunction;
    status = napi_create_function(env, "pathElementCount", NAPI_AUTO_LENGTH, pathElementCount, NULL, &pathElementCountFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create native function");
    }
    status = napi_set_named_property(env, exports, "pathElementCount", pathElementCountFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to add native function to exports");
    }

    napi_value pathCurrentPointFunction;
    status = napi_create_function(env, "pathCurrentPoint", NAPI_AUTO_LENGTH, pathCurrentPoint, NULL, &pathCurrentPointFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create native function");
    }
    status = napi_set_named_property(env, exports, "pathCurrentPoint", pathCurrentPointFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to add native function to exports");
    }

    napi_value pathBoundsFunction;
    status = napi_create_function(env, "pathBounds", NAPI_AUTO_LENGTH, pathBounds, NULL, &pathBoundsFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create native function");
    }
    status = napi_set_named_property(env, exports, "pathBounds", pathBoundsFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to add native function to exports");
    }

    napi_value pathMoveFunction;
    status = napi_create_function(env, "pathMove", NAPI_AUTO_LENGTH, pathMove, NULL, &pathMoveFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create native function");
    }
    status = napi_set_named_property(env, exports, "pathMove", pathMoveFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to add native function to exports");
    }

    napi_value pathAddLineFunction;
    status = napi_create_function(env, "pathAddLine", NAPI_AUTO_LENGTH, pathAddLine, NULL, &pathAddLineFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create native function");
    }
    status = napi_set_named_property(env, exports, "pathAddLine", pathAddLineFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to add native function to exports");
    }

    napi_value pathAddQuadraticCurveFunction;
    status = napi_create_function(env, "pathAddQuadraticCurve", NAPI_AUTO_LENGTH, pathAddQuadraticCurve, NULL, &pathAddQuadraticCurveFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create native function");
    }
    status = napi_set_named_property(env, exports, "pathAddQuadraticCurve", pathAddQuadraticCurveFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to add native function to exports");
    }

    napi_value pathAddCubicCurveFunction;
    status = napi_create_function(env, "pathAddCubicCurve", NAPI_AUTO_LENGTH, pathAddCubicCurve, NULL, &pathAddCubicCurveFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create native function");
    }
    status = napi_set_named_property(env, exports, "pathAddCubicCurve", pathAddCubicCurveFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to add native function to exports");
    }

    napi_value pathCloseFunction;
    status = napi_create_function(env, "pathClose", NAPI_AUTO_LENGTH, pathClose, NULL, &pathCloseFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create native function");
    }
    status = napi_set_named_property(env, exports, "pathClose", pathCloseFunction);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to add native function to exports");
    }

    return exports;
}

NAPI_MODULE_INIT() {
    return Init(env, exports);
}
