/* Include files */

#include "Ensamblaje_propio_sfun.h"
#include "c2_Ensamblaje_propio.h"
#include <math.h>
#include <string.h>
#include "mwmathutil.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c(S);
#ifdef utFree
#undef utFree
#endif

#ifdef utMalloc
#undef utMalloc
#endif

#ifdef __cplusplus

extern "C" void *utMalloc(size_t size);
extern "C" void utFree(void*);

#else

extern void *utMalloc(size_t size);
extern void utFree(void*);

#endif

/* Forward Declarations */

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)

/* Variable Declarations */

/* Variable Definitions */
static real_T _sfTime_;
static emlrtMCInfo c2_emlrtMCI = { 55, /* lineNo */
  5,                                   /* colNo */
  "charCastCheck",                     /* fName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\eml\\+coder\\+internal\\charCastCheck.m"/* pName */
};

static emlrtMCInfo c2_b_emlrtMCI = { 85,/* lineNo */
  23,                                  /* colNo */
  "reshapeSizeChecks",                 /* fName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\eml\\+coder\\+internal\\reshapeSizeChecks.m"/* pName */
};

static emlrtMCInfo c2_c_emlrtMCI = { 87,/* lineNo */
  23,                                  /* colNo */
  "reshapeSizeChecks",                 /* fName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\eml\\+coder\\+internal\\reshapeSizeChecks.m"/* pName */
};

static emlrtMCInfo c2_d_emlrtMCI = { 58,/* lineNo */
  23,                                  /* colNo */
  "assertValidSizeArg",                /* fName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\eml\\+coder\\+internal\\assertValidSizeArg.m"/* pName */
};

static emlrtRSInfo c2_emlrtRSI = { 2,  /* lineNo */
  "MATLAB Function",                   /* fcnName */
  "#Ensamblaje_propio:119"             /* pathName */
};

static emlrtRSInfo c2_b_emlrtRSI = { 4,/* lineNo */
  "MATLAB Function",                   /* fcnName */
  "#Ensamblaje_propio:119"             /* pathName */
};

static emlrtRSInfo c2_c_emlrtRSI = { 28,/* lineNo */
  "char",                              /* fcnName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\lib\\matlab\\strfun\\char.m"/* pathName */
};

static emlrtRSInfo c2_d_emlrtRSI = { 39,/* lineNo */
  "charCastCheck",                     /* fcnName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\eml\\+coder\\+internal\\charCastCheck.m"/* pathName */
};

static emlrtRSInfo c2_e_emlrtRSI = { 26,/* lineNo */
  "sscanf",                            /* fcnName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\lib\\matlab\\strfun\\sscanf.m"/* pathName */
};

static emlrtRSInfo c2_f_emlrtRSI = { 45,/* lineNo */
  "sscanf",                            /* fcnName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\lib\\matlab\\strfun\\sscanf.m"/* pathName */
};

static emlrtRSInfo c2_g_emlrtRSI = { 51,/* lineNo */
  "reshapeSizeChecks",                 /* fcnName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\eml\\+coder\\+internal\\reshapeSizeChecks.m"/* pathName */
};

static emlrtRSInfo c2_h_emlrtRSI = { 113,/* lineNo */
  "reshapeSizeChecks",                 /* fcnName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\eml\\+coder\\+internal\\reshapeSizeChecks.m"/* pathName */
};

static emlrtRSInfo c2_i_emlrtRSI = { 54,/* lineNo */
  "sscanf",                            /* fcnName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\lib\\matlab\\strfun\\sscanf.m"/* pathName */
};

static emlrtRSInfo c2_j_emlrtRSI = { 79,/* lineNo */
  "scanf",                             /* fcnName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\eml\\+coder\\+internal\\+io\\scanf.m"/* pathName */
};

static emlrtRTEInfo c2_emlrtRTEI = { 3,/* lineNo */
  5,                                   /* colNo */
  "MATLAB Function",                   /* fName */
  "#Ensamblaje_propio:119"             /* pName */
};

static emlrtRTEInfo c2_b_emlrtRTEI = { 4,/* lineNo */
  19,                                  /* colNo */
  "MATLAB Function",                   /* fName */
  "#Ensamblaje_propio:119"             /* pName */
};

static emlrtRTEInfo c2_c_emlrtRTEI = { 25,/* lineNo */
  18,                                  /* colNo */
  "sscanf",                            /* fName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\lib\\matlab\\strfun\\sscanf.m"/* pName */
};

static emlrtRTEInfo c2_d_emlrtRTEI = { 26,/* lineNo */
  1,                                   /* colNo */
  "sscanf",                            /* fName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\lib\\matlab\\strfun\\sscanf.m"/* pName */
};

static emlrtRTEInfo c2_e_emlrtRTEI = { 27,/* lineNo */
  1,                                   /* colNo */
  "sscanf",                            /* fName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\lib\\matlab\\strfun\\sscanf.m"/* pName */
};

static emlrtRTEInfo c2_f_emlrtRTEI = { 29,/* lineNo */
  9,                                   /* colNo */
  "scanf",                             /* fName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\eml\\+coder\\+internal\\+io\\scanf.m"/* pName */
};

static emlrtRTEInfo c2_g_emlrtRTEI = { 79,/* lineNo */
  67,                                  /* colNo */
  "scanf",                             /* fName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\eml\\+coder\\+internal\\+io\\scanf.m"/* pName */
};

static emlrtRTEInfo c2_h_emlrtRTEI = { 90,/* lineNo */
  37,                                  /* colNo */
  "scanf",                             /* fName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\eml\\+coder\\+internal\\+io\\scanf.m"/* pName */
};

static emlrtRTEInfo c2_i_emlrtRTEI = { 27,/* lineNo */
  9,                                   /* colNo */
  "scanf",                             /* fName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\eml\\+coder\\+internal\\+io\\scanf.m"/* pName */
};

static emlrtBCInfo c2_emlrtBCI = { 0,  /* iFirst */
  MAX_int32_T,                         /* iLast */
  5,                                   /* lineNo */
  8,                                   /* colNo */
  "vals",                              /* aName */
  "MATLAB Function",                   /* fName */
  "#Ensamblaje_propio:119",            /* pName */
  0                                    /* checkKind */
};

static emlrtBCInfo c2_b_emlrtBCI = { 0,/* iFirst */
  0,                                   /* iLast */
  3,                                   /* lineNo */
  15,                                  /* colNo */
  "str",                               /* aName */
  "MATLAB Function",                   /* fName */
  "#Ensamblaje_propio:119",            /* pName */
  0                                    /* checkKind */
};

static emlrtDCInfo c2_emlrtDCI = { 3,  /* lineNo */
  15,                                  /* colNo */
  "MATLAB Function",                   /* fName */
  "#Ensamblaje_propio:119",            /* pName */
  1                                    /* checkKind */
};

static emlrtBCInfo c2_c_emlrtBCI = { -1,/* iFirst */
  -1,                                  /* iLast */
  6,                                   /* lineNo */
  18,                                  /* colNo */
  "vals",                              /* aName */
  "MATLAB Function",                   /* fName */
  "#Ensamblaje_propio:119",            /* pName */
  0                                    /* checkKind */
};

static emlrtBCInfo c2_d_emlrtBCI = { -1,/* iFirst */
  -1,                                  /* iLast */
  6,                                   /* lineNo */
  31,                                  /* colNo */
  "vals",                              /* aName */
  "MATLAB Function",                   /* fName */
  "#Ensamblaje_propio:119",            /* pName */
  0                                    /* checkKind */
};

static emlrtBCInfo c2_e_emlrtBCI = { -1,/* iFirst */
  -1,                                  /* iLast */
  6,                                   /* lineNo */
  44,                                  /* colNo */
  "vals",                              /* aName */
  "MATLAB Function",                   /* fName */
  "#Ensamblaje_propio:119",            /* pName */
  0                                    /* checkKind */
};

static emlrtBCInfo c2_f_emlrtBCI = { -1,/* iFirst */
  -1,                                  /* iLast */
  6,                                   /* lineNo */
  57,                                  /* colNo */
  "vals",                              /* aName */
  "MATLAB Function",                   /* fName */
  "#Ensamblaje_propio:119",            /* pName */
  0                                    /* checkKind */
};

static emlrtBCInfo c2_g_emlrtBCI = { -1,/* iFirst */
  -1,                                  /* iLast */
  6,                                   /* lineNo */
  70,                                  /* colNo */
  "vals",                              /* aName */
  "MATLAB Function",                   /* fName */
  "#Ensamblaje_propio:119",            /* pName */
  0                                    /* checkKind */
};

/* Function Declarations */
static void initialize_c2_Ensamblaje_propio(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance);
static void initialize_params_c2_Ensamblaje_propio
  (SFc2_Ensamblaje_propioInstanceStruct *chartInstance);
static void mdl_start_c2_Ensamblaje_propio(SFc2_Ensamblaje_propioInstanceStruct *
  chartInstance);
static void mdl_terminate_c2_Ensamblaje_propio
  (SFc2_Ensamblaje_propioInstanceStruct *chartInstance);
static void mdl_setup_runtime_resources_c2_Ensamblaje_propio
  (SFc2_Ensamblaje_propioInstanceStruct *chartInstance);
static void mdl_cleanup_runtime_resources_c2_Ensamblaje_propio
  (SFc2_Ensamblaje_propioInstanceStruct *chartInstance);
static void enable_c2_Ensamblaje_propio(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance);
static void disable_c2_Ensamblaje_propio(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance);
static void sf_gateway_c2_Ensamblaje_propio(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance);
static void ext_mode_exec_c2_Ensamblaje_propio
  (SFc2_Ensamblaje_propioInstanceStruct *chartInstance);
static void c2_update_jit_animation_c2_Ensamblaje_propio
  (SFc2_Ensamblaje_propioInstanceStruct *chartInstance);
static void c2_do_animation_call_c2_Ensamblaje_propio
  (SFc2_Ensamblaje_propioInstanceStruct *chartInstance);
static const mxArray *get_sim_state_c2_Ensamblaje_propio
  (SFc2_Ensamblaje_propioInstanceStruct *chartInstance);
static void set_sim_state_c2_Ensamblaje_propio
  (SFc2_Ensamblaje_propioInstanceStruct *chartInstance, const mxArray *c2_st);
static void c2_indexShapeCheck(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance);
static real_T c2_emlrt_marshallIn(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance, const mxArray *c2_nullptr, const char_T *c2_identifier);
static real_T c2_b_emlrt_marshallIn(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_array_real_T_SetSize(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance, const emlrtStack *c2_sp, c2_coder_array_real_T *c2_coderArray,
  const emlrtRTEInfo *c2_srcLocation, int32_T c2_size0);
static void c2_array_real_T_Constructor(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance, c2_coder_array_real_T *c2_coderArray);
static void c2_array_real_T_Destructor(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance, c2_coder_array_real_T *c2_coderArray);
static uint8_T c2__u8_s8_(SFc2_Ensamblaje_propioInstanceStruct *chartInstance,
  int8_T c2_b_b, int32_T c2_EMLOvCount_src_loc, uint32_T c2_ssid_src_loc,
  int32_T c2_offset_src_loc, int32_T c2_length_src_loc);
static void init_dsm_address_info(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance);
static void init_simulink_io_address(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance);

/* Function Definitions */
static void initialize_c2_Ensamblaje_propio(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance)
{
  sim_mode_is_external(chartInstance->S);
  chartInstance->c2_doneDoubleBufferReInit = false;
  chartInstance->c2_sfEvent = CALL_EVENT;
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void initialize_params_c2_Ensamblaje_propio
  (SFc2_Ensamblaje_propioInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void mdl_start_c2_Ensamblaje_propio(SFc2_Ensamblaje_propioInstanceStruct *
  chartInstance)
{
  sim_mode_is_external(chartInstance->S);
}

static void mdl_terminate_c2_Ensamblaje_propio
  (SFc2_Ensamblaje_propioInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void mdl_setup_runtime_resources_c2_Ensamblaje_propio
  (SFc2_Ensamblaje_propioInstanceStruct *chartInstance)
{
  static const uint32_T c2_decisionTxtEndIdx = 0U;
  static const uint32_T c2_decisionTxtStartIdx = 0U;
  sfSetAnimationVectors(chartInstance->S, &chartInstance->c2_JITStateAnimation[0],
                        &chartInstance->c2_JITTransitionAnimation[0]);
  covrtCreateStateflowInstanceData(chartInstance->c2_covrtInstance, 1U, 0U, 1U,
    12U);
  covrtChartInitFcn(chartInstance->c2_covrtInstance, 0U, false, false, false);
  covrtStateInitFcn(chartInstance->c2_covrtInstance, 0U, 0U, false, false, false,
                    0U, &c2_decisionTxtStartIdx, &c2_decisionTxtEndIdx);
  covrtTransInitFcn(chartInstance->c2_covrtInstance, 0U, 0, NULL, NULL, 0U, NULL);
  covrtEmlInitFcn(chartInstance->c2_covrtInstance, "", 4U, 0U, 1U, 0U, 1U, 0U,
                  0U, 0U, 0U, 0U, 0U, 0U);
  covrtEmlFcnInitFcn(chartInstance->c2_covrtInstance, 4U, 0U, 0U,
                     "c2_Ensamblaje_propio", 0, -1, 293);
  covrtEmlIfInitFcn(chartInstance->c2_covrtInstance, 4U, 0U, 0U, 127, 146, 224,
                    289, false);
  covrtEmlRelationalInitFcn(chartInstance->c2_covrtInstance, 4U, 0U, 0U, 130,
    146, -1, 0U);
}

static void mdl_cleanup_runtime_resources_c2_Ensamblaje_propio
  (SFc2_Ensamblaje_propioInstanceStruct *chartInstance)
{
  covrtDeleteStateflowInstanceData(chartInstance->c2_covrtInstance);
}

static void enable_c2_Ensamblaje_propio(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void disable_c2_Ensamblaje_propio(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void sf_gateway_c2_Ensamblaje_propio(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance)
{
  static int32_T c2_outCell[6] = { 0, 0, 0, 0, 0, -1 };

  static char_T c2_cv2[40] = { 'C', 'o', 'd', 'e', 'r', ':', 'M', 'A', 'T', 'L',
    'A', 'B', ':', 'g', 'e', 't', 'R', 'e', 's', 'h', 'a', 'p', 'e', 'D', 'i',
    'm', 's', '_', 'n', 'o', 't', 'S', 'a', 'm', 'e', 'N', 'u', 'm', 'e', 'l' };

  static char_T c2_cv[30] = { 'C', 'o', 'd', 'e', 'r', ':', 't', 'o', 'o', 'l',
    'b', 'o', 'x', ':', 'O', 'u', 't', 'O', 'f', 'T', 'a', 'r', 'g', 'e', 't',
    'R', 'a', 'n', 'g', 'e' };

  static char_T c2_cv1[28] = { 'C', 'o', 'd', 'e', 'r', ':', 'M', 'A', 'T', 'L',
    'A', 'B', ':', 'N', 'o', 'n', 'I', 'n', 't', 'e', 'g', 'e', 'r', 'I', 'n',
    'p', 'u', 't' };

  c2_coder_array_real_T c2_bigA;
  emlrtStack c2_b_st;
  emlrtStack c2_c_st;
  emlrtStack c2_d_st;
  emlrtStack c2_e_st;
  emlrtStack c2_st = { NULL,           /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  const mxArray *c2_b_y = NULL;
  const mxArray *c2_c_y = NULL;
  const mxArray *c2_d_y = NULL;
  const mxArray *c2_e_y = NULL;
  const mxArray *c2_f_y = NULL;
  const mxArray *c2_g_y = NULL;
  const mxArray *c2_h_y = NULL;
  const mxArray *c2_i_y = NULL;
  const mxArray *c2_j_y = NULL;
  const mxArray *c2_y = NULL;
  real_T c2_arg;
  real_T c2_b_a;
  real_T c2_b_c;
  real_T c2_b_d;
  real_T c2_b_e;
  real_T c2_b_u;
  real_T c2_b_varargin_1;
  real_T c2_b_varargin_2;
  real_T c2_b_x;
  real_T c2_c_b;
  real_T c2_c_d;
  real_T c2_c_u;
  real_T c2_c_x;
  real_T c2_d1;
  real_T c2_d10;
  real_T c2_d11;
  real_T c2_d12;
  real_T c2_d13;
  real_T c2_d2;
  real_T c2_d3;
  real_T c2_d4;
  real_T c2_d5;
  real_T c2_d6;
  real_T c2_d7;
  real_T c2_d8;
  real_T c2_d9;
  real_T c2_d_u;
  real_T c2_varargin_1;
  real_T c2_varargin_2;
  real_T c2_x;
  int32_T c2_tmpOut[6];
  int32_T c2_inStr_size[2];
  int32_T c2_iv[2];
  int32_T c2_str_size[2];
  int32_T c2_sz[2];
  int32_T c2_tmp_size[2];
  int32_T c2_b_i;
  int32_T c2_b_loop_ub;
  int32_T c2_b_numRead;
  int32_T c2_c_i;
  int32_T c2_c_loop_ub;
  int32_T c2_c_varargin_2;
  int32_T c2_d_loop_ub;
  int32_T c2_d_varargin_2;
  int32_T c2_e_loop_ub;
  int32_T c2_e_u;
  int32_T c2_end;
  int32_T c2_f_loop_ub;
  int32_T c2_f_u;
  int32_T c2_i;
  int32_T c2_i10;
  int32_T c2_i11;
  int32_T c2_i12;
  int32_T c2_i13;
  int32_T c2_i14;
  int32_T c2_i15;
  int32_T c2_i16;
  int32_T c2_i17;
  int32_T c2_i18;
  int32_T c2_i19;
  int32_T c2_i20;
  int32_T c2_i21;
  int32_T c2_i22;
  int32_T c2_i23;
  int32_T c2_i24;
  int32_T c2_i25;
  int32_T c2_i3;
  int32_T c2_i4;
  int32_T c2_i5;
  int32_T c2_i6;
  int32_T c2_i7;
  int32_T c2_i8;
  int32_T c2_i9;
  int32_T c2_loop_ub;
  int32_T c2_numRead;
  int32_T c2_nx;
  int32_T c2_ptrLoc;
  int32_T c2_result;
  int32_T c2_sumptrLoc;
  int32_T c2_trueCount;
  char_T c2_fileString_data[2];
  char_T c2_inStr_data[2];
  char_T c2_str_data[1];
  int8_T c2_i1;
  int8_T c2_i2;
  boolean_T c2_b_b;
  boolean_T c2_exitg1;
  boolean_T c2_incompleteRead;
  boolean_T c2_p;
  c2_st.tls = chartInstance->c2_fEmlrtCtx;
  c2_b_st.prev = &c2_st;
  c2_b_st.tls = c2_st.tls;
  c2_c_st.prev = &c2_b_st;
  c2_c_st.tls = c2_b_st.tls;
  c2_d_st.prev = &c2_c_st;
  c2_d_st.tls = c2_c_st.tls;
  c2_e_st.prev = &c2_d_st;
  c2_e_st.tls = c2_d_st.tls;
  covrtSigUpdateFcn(chartInstance->c2_covrtInstance, 0U, *chartInstance->c2_u);
  _sfTime_ = sf_get_time(chartInstance->S);
  chartInstance->c2_JITTransitionAnimation[0] = 0U;
  chartInstance->c2_sfEvent = CALL_EVENT;
  c2_b_u = *chartInstance->c2_u;
  covrtEmlFcnEval(chartInstance->c2_covrtInstance, 4U, 0, 0);
  c2_b_st.site = &c2_emlrtRSI;
  c2_varargin_1 = c2_b_u;
  c2_c_st.site = &c2_c_emlrtRSI;
  c2_x = c2_varargin_1;
  c2_b_x = c2_x;
  c2_d_st.site = &c2_d_emlrtRSI;
  c2_c_x = c2_b_x;
  if ((c2_c_x >= 0.0) && (c2_c_x <= 255.0)) {
  } else {
    c2_y = NULL;
    sf_mex_assign(&c2_y, sf_mex_create("y", c2_cv, 10, 0U, 1, 0U, 2, 1, 30),
                  false);
    c2_b_y = NULL;
    sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_cv, 10, 0U, 1, 0U, 2, 1, 30),
                  false);
    c2_c_u = 0.0;
    c2_c_y = NULL;
    sf_mex_assign(&c2_c_y, sf_mex_create("y", &c2_c_u, 0, 0U, 0, 0U, 0), false);
    c2_d_u = 255.0;
    c2_d_y = NULL;
    sf_mex_assign(&c2_d_y, sf_mex_create("y", &c2_d_u, 0, 0U, 0, 0U, 0), false);
    sf_mex_call(&c2_d_st, &c2_emlrtMCI, "error", 0U, 2U, 14, c2_y, 14,
                sf_mex_call(&c2_d_st, NULL, "getString", 1U, 1U, 14, sf_mex_call
      (&c2_d_st, NULL, "message", 1U, 3U, 14, c2_b_y, 14, c2_c_y, 14, c2_d_y)));
  }

  c2_end = 1;
  c2_trueCount = 0;
  for (c2_i = 0; c2_i < c2_end; c2_i++) {
    c2_b_d = trunc(c2_b_x);
    if (muDoubleScalarIsNaN(c2_b_d) || muDoubleScalarIsInf(c2_b_d)) {
      c2_d1 = 0.0;
    } else {
      c2_d1 = muDoubleScalarRem(c2_b_d, 256.0);
    }

    if (c2_d1 < 0.0) {
      c2_i1 = (int8_T)-(int8_T)(uint8_T)-c2_d1;
    } else {
      c2_i1 = (int8_T)(uint8_T)c2_d1;
    }

    if (c2__u8_s8_(chartInstance, c2_i1, 0, 1U, 70, 8) != 0) {
      c2_trueCount++;
    }
  }

  c2_tmp_size[1] = c2_trueCount;
  for (c2_b_i = 0; c2_b_i < c2_end; c2_b_i++) {
    c2_d2 = trunc(c2_b_x);
    if (muDoubleScalarIsNaN(c2_d2) || muDoubleScalarIsInf(c2_d2)) {
      c2_d3 = 0.0;
    } else {
      c2_d3 = muDoubleScalarRem(c2_d2, 256.0);
    }

    if (c2_d3 < 0.0) {
      c2_i2 = (int8_T)-(int8_T)(uint8_T)-c2_d3;
    } else {
      c2_i2 = (int8_T)(uint8_T)c2_d3;
    }

    if (c2__u8_s8_(chartInstance, c2_i2, 0, 1U, 70, 8) != 0) {
      c2_d6 = (real_T)c2_b_i;
      if (c2_d6 != (real_T)(int32_T)muDoubleScalarFloor(c2_d6)) {
        emlrtIntegerCheckR2012b(c2_d6, &c2_emlrtDCI, &c2_st);
      }

      c2_i5 = (int32_T)muDoubleScalarFloor(c2_d6);
      if ((c2_i5 < 0) || (c2_i5 > 0)) {
        emlrtDynamicBoundsCheckR2012b(c2_i5, 0, 0, &c2_b_emlrtBCI, &c2_st);
      }
    }
  }

  c2_str_size[1] = c2_tmp_size[1];
  c2_loop_ub = c2_tmp_size[1] - 1;
  for (c2_c_i = 0; c2_c_i <= c2_loop_ub; c2_c_i++) {
    c2_d4 = trunc(c2_b_x);
    if (muDoubleScalarIsNaN(c2_d4) || muDoubleScalarIsInf(c2_d4)) {
      c2_d5 = 0.0;
    } else {
      c2_d5 = muDoubleScalarRem(c2_d4, 256.0);
    }

    if (c2_d5 < 0.0) {
      c2_str_data[0] = (char_T)(int8_T)-(int8_T)(uint8_T)-c2_d5;
    } else {
      c2_str_data[0] = (char_T)(int8_T)(uint8_T)c2_d5;
    }
  }

  c2_b_st.site = &c2_b_emlrtRSI;
  c2_inStr_size[1] = c2_str_size[1];
  c2_b_loop_ub = c2_str_size[1] - 1;
  for (c2_i3 = 0; c2_i3 <= c2_b_loop_ub; c2_i3++) {
    c2_inStr_data[c2_i3] = c2_str_data[0];
  }

  c2_str_size[1] = c2_inStr_size[1];
  c2_c_loop_ub = c2_inStr_size[1] - 1;
  for (c2_i4 = 0; c2_i4 <= c2_c_loop_ub; c2_i4++) {
    c2_str_data[0] = c2_inStr_data[0];
  }

  c2_d7 = (real_T)c2_str_size[1];
  c2_c_st.site = &c2_e_emlrtRSI;
  c2_varargin_2 = c2_d7;
  c2_nx = c2_str_size[1];
  c2_d_st.site = &c2_g_emlrtRSI;
  c2_b_varargin_2 = c2_varargin_2;
  c2_e_st.site = &c2_h_emlrtRSI;
  c2_b_varargin_1 = c2_b_varargin_2;
  c2_arg = c2_b_varargin_1;
  if (c2_arg != c2_arg) {
    c2_p = false;
  } else {
    c2_p = true;
  }

  if (c2_p) {
    c2_b_b = true;
  } else {
    c2_b_b = false;
  }

  if (!c2_b_b) {
    c2_e_y = NULL;
    sf_mex_assign(&c2_e_y, sf_mex_create("y", c2_cv1, 10, 0U, 1, 0U, 2, 1, 28),
                  false);
    c2_f_y = NULL;
    sf_mex_assign(&c2_f_y, sf_mex_create("y", c2_cv1, 10, 0U, 1, 0U, 2, 1, 28),
                  false);
    c2_e_u = MIN_int32_T;
    c2_g_y = NULL;
    sf_mex_assign(&c2_g_y, sf_mex_create("y", &c2_e_u, 6, 0U, 0, 0U, 0), false);
    c2_f_u = MAX_int32_T;
    c2_j_y = NULL;
    sf_mex_assign(&c2_j_y, sf_mex_create("y", &c2_f_u, 6, 0U, 0, 0U, 0), false);
    sf_mex_call(&c2_e_st, &c2_d_emlrtMCI, "error", 0U, 2U, 14, c2_e_y, 14,
                sf_mex_call(&c2_e_st, NULL, "getString", 1U, 1U, 14, sf_mex_call
      (&c2_e_st, NULL, "message", 1U, 3U, 14, c2_f_y, 14, c2_g_y, 14, c2_j_y)));
  }

  c2_sz[1] = (int32_T)c2_varargin_2;
  c2_c_varargin_2 = c2_sz[1];
  c2_d_varargin_2 = c2_c_varargin_2;
  c2_result = c2_d_varargin_2;
  if (c2_result != c2_nx) {
    c2_h_y = NULL;
    sf_mex_assign(&c2_h_y, sf_mex_create("y", c2_cv2, 10, 0U, 1, 0U, 2, 1, 40),
                  false);
    c2_i_y = NULL;
    sf_mex_assign(&c2_i_y, sf_mex_create("y", c2_cv2, 10, 0U, 1, 0U, 2, 1, 40),
                  false);
    sf_mex_call(&c2_c_st, &c2_c_emlrtMCI, "error", 0U, 2U, 14, c2_h_y, 14,
                sf_mex_call(&c2_c_st, NULL, "getString", 1U, 1U, 14, sf_mex_call
      (&c2_c_st, NULL, "message", 1U, 1U, 14, c2_i_y)));
  }

  c2_iv[1] = (int32_T)c2_d7;
  c2_str_size[1] = c2_iv[1];
  c2_d_loop_ub = c2_iv[1] - 1;
  for (c2_i6 = 0; c2_i6 <= c2_d_loop_ub; c2_i6++) {
    c2_str_data[c2_i6] = c2_str_data[0];
  }

  c2_inStr_size[1] = c2_str_size[1] + 1;
  c2_e_loop_ub = c2_str_size[1] - 1;
  for (c2_i7 = 0; c2_i7 <= c2_e_loop_ub; c2_i7++) {
    c2_inStr_data[c2_i7] = c2_str_data[c2_i7];
  }

  c2_inStr_data[c2_str_size[1]] = '\x00';
  c2_c_st.site = &c2_f_emlrtRSI;
  c2_d_st.site = &c2_i_emlrtRSI;
  c2_array_real_T_Constructor(chartInstance, &c2_bigA);
  c2_array_real_T_SetSize(chartInstance, &c2_d_st, &c2_bigA, &c2_f_emlrtRTEI, 0);
  c2_numRead = 1;
  c2_sumptrLoc = 0;
  c2_exitg1 = false;
  while ((!c2_exitg1) && ((c2_numRead > 0) && (!(c2_sumptrLoc >= c2_inStr_size[1]))))
  {
    c2_e_st.site = &c2_j_emlrtRSI;
    c2_f_loop_ub = c2_inStr_size[1] - 1;
    for (c2_i9 = 0; c2_i9 <= c2_f_loop_ub; c2_i9++) {
      c2_fileString_data[c2_i9] = c2_inStr_data[c2_i9];
    }

    c2_ptrLoc = c2_sumptrLoc;
    for (c2_i11 = 0; c2_i11 < 6; c2_i11++) {
      c2_tmpOut[c2_i11] = c2_outCell[c2_i11];
    }

    c2_tmpOut[5] = -1;
    c2_b_numRead = sscanf(&c2_fileString_data[c2_ptrLoc], "%d,%d,%d,%d,%d%n",
                          &c2_tmpOut[0], &c2_tmpOut[1], &c2_tmpOut[2],
                          &c2_tmpOut[3], &c2_tmpOut[4], &c2_tmpOut[5]);
    if (c2_tmpOut[5] != -1) {
      c2_b_numRead++;
    }

    c2_numRead = c2_b_numRead;
    c2_incompleteRead = (c2_b_numRead == 0);
    if (c2_b_numRead > 0) {
      c2_d9 = (real_T)c2_tmpOut[0];
      c2_i16 = c2_bigA.size[0];
      c2_array_real_T_SetSize(chartInstance, &c2_d_st, &c2_bigA, &c2_h_emlrtRTEI,
        c2_bigA.size[0] + 1);
      c2_bigA.vector.data[c2_i16] = c2_d9;
      if (c2_b_numRead >= 2) {
        c2_d10 = (real_T)c2_tmpOut[1];
        c2_i19 = c2_bigA.size[0];
        c2_array_real_T_SetSize(chartInstance, &c2_d_st, &c2_bigA,
          &c2_h_emlrtRTEI, c2_bigA.size[0] + 1);
        c2_bigA.vector.data[c2_i19] = c2_d10;
      } else {
        c2_incompleteRead = true;
      }

      if (c2_b_numRead >= 3) {
        c2_d11 = (real_T)c2_tmpOut[2];
        c2_i21 = c2_bigA.size[0];
        c2_array_real_T_SetSize(chartInstance, &c2_d_st, &c2_bigA,
          &c2_h_emlrtRTEI, c2_bigA.size[0] + 1);
        c2_bigA.vector.data[c2_i21] = c2_d11;
      } else {
        c2_incompleteRead = true;
      }

      if (c2_b_numRead >= 4) {
        c2_d12 = (real_T)c2_tmpOut[3];
        c2_i24 = c2_bigA.size[0];
        c2_array_real_T_SetSize(chartInstance, &c2_d_st, &c2_bigA,
          &c2_h_emlrtRTEI, c2_bigA.size[0] + 1);
        c2_bigA.vector.data[c2_i24] = c2_d12;
      } else {
        c2_incompleteRead = true;
      }

      if (c2_b_numRead >= 5) {
        c2_d13 = (real_T)c2_tmpOut[4];
        c2_i25 = c2_bigA.size[0];
        c2_array_real_T_SetSize(chartInstance, &c2_d_st, &c2_bigA,
          &c2_h_emlrtRTEI, c2_bigA.size[0] + 1);
        c2_bigA.vector.data[c2_i25] = c2_d13;
      } else {
        c2_incompleteRead = true;
      }

      if (c2_b_numRead >= 6) {
        c2_sumptrLoc += c2_tmpOut[5];
      } else {
        c2_incompleteRead = true;
      }
    }

    if (c2_incompleteRead) {
      c2_exitg1 = true;
    }
  }

  c2_i8 = c2_bigA.size[0];
  if (c2_i8 < 0) {
    emlrtDynamicBoundsCheckR2012b(c2_i8, 0, MAX_int32_T, &c2_emlrtBCI, &c2_st);
  }

  c2_d8 = (real_T)c2_i8;
  if (covrtEmlIfEval(chartInstance->c2_covrtInstance, 4U, 0, 0,
                     covrtRelationalopUpdateFcn(chartInstance->c2_covrtInstance,
        4U, 0U, 0U, c2_d8, 5.0, -1, 0U, c2_d8 == 5.0))) {
    c2_i10 = c2_bigA.size[0];
    c2_i12 = 1;
    if ((c2_i12 < 1) || (c2_i12 > c2_i10)) {
      emlrtDynamicBoundsCheckR2012b(c2_i12, 1, c2_i10, &c2_c_emlrtBCI, &c2_st);
    }

    c2_b_a = c2_bigA.vector.data[c2_i12 - 1];
    c2_i13 = c2_bigA.size[0];
    c2_i14 = 2;
    if ((c2_i14 < 1) || (c2_i14 > c2_i13)) {
      emlrtDynamicBoundsCheckR2012b(c2_i14, 1, c2_i13, &c2_d_emlrtBCI, &c2_st);
    }

    c2_c_b = c2_bigA.vector.data[c2_i14 - 1];
    c2_i15 = c2_bigA.size[0];
    c2_i17 = 3;
    if ((c2_i17 < 1) || (c2_i17 > c2_i15)) {
      emlrtDynamicBoundsCheckR2012b(c2_i17, 1, c2_i15, &c2_e_emlrtBCI, &c2_st);
    }

    c2_b_c = c2_bigA.vector.data[c2_i17 - 1];
    c2_i18 = c2_bigA.size[0];
    c2_i20 = 4;
    if ((c2_i20 < 1) || (c2_i20 > c2_i18)) {
      emlrtDynamicBoundsCheckR2012b(c2_i20, 1, c2_i18, &c2_f_emlrtBCI, &c2_st);
    }

    c2_c_d = c2_bigA.vector.data[c2_i20 - 1];
    c2_i22 = c2_bigA.size[0];
    c2_i23 = 5;
    if ((c2_i23 < 1) || (c2_i23 > c2_i22)) {
      emlrtDynamicBoundsCheckR2012b(c2_i23, 1, c2_i22, &c2_g_emlrtBCI, &c2_st);
    }

    c2_b_e = c2_bigA.vector.data[c2_i23 - 1];
  } else {
    c2_b_a = rtNaN;
    c2_c_b = rtNaN;
    c2_b_c = rtNaN;
    c2_c_d = rtNaN;
    c2_b_e = rtNaN;
  }

  c2_array_real_T_Destructor(chartInstance, &c2_bigA);
  *chartInstance->c2_a = c2_b_a;
  *chartInstance->c2_b = c2_c_b;
  *chartInstance->c2_c = c2_b_c;
  *chartInstance->c2_d = c2_c_d;
  *chartInstance->c2_e = c2_b_e;
  covrtSigUpdateFcn(chartInstance->c2_covrtInstance, 1U, *chartInstance->c2_a);
  covrtSigUpdateFcn(chartInstance->c2_covrtInstance, 2U, *chartInstance->c2_b);
  covrtSigUpdateFcn(chartInstance->c2_covrtInstance, 3U, *chartInstance->c2_c);
  covrtSigUpdateFcn(chartInstance->c2_covrtInstance, 4U, *chartInstance->c2_d);
  covrtSigUpdateFcn(chartInstance->c2_covrtInstance, 5U, *chartInstance->c2_e);
}

static void ext_mode_exec_c2_Ensamblaje_propio
  (SFc2_Ensamblaje_propioInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void c2_update_jit_animation_c2_Ensamblaje_propio
  (SFc2_Ensamblaje_propioInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void c2_do_animation_call_c2_Ensamblaje_propio
  (SFc2_Ensamblaje_propioInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static const mxArray *get_sim_state_c2_Ensamblaje_propio
  (SFc2_Ensamblaje_propioInstanceStruct *chartInstance)
{
  const mxArray *c2_b_y = NULL;
  const mxArray *c2_c_y = NULL;
  const mxArray *c2_d_y = NULL;
  const mxArray *c2_e_y = NULL;
  const mxArray *c2_f_y = NULL;
  const mxArray *c2_st;
  const mxArray *c2_y = NULL;
  c2_st = NULL;
  c2_st = NULL;
  c2_y = NULL;
  sf_mex_assign(&c2_y, sf_mex_createcellmatrix(5, 1), false);
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", chartInstance->c2_a, 0, 0U, 0, 0U, 0),
                false);
  sf_mex_setcell(c2_y, 0, c2_b_y);
  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", chartInstance->c2_b, 0, 0U, 0, 0U, 0),
                false);
  sf_mex_setcell(c2_y, 1, c2_c_y);
  c2_d_y = NULL;
  sf_mex_assign(&c2_d_y, sf_mex_create("y", chartInstance->c2_c, 0, 0U, 0, 0U, 0),
                false);
  sf_mex_setcell(c2_y, 2, c2_d_y);
  c2_e_y = NULL;
  sf_mex_assign(&c2_e_y, sf_mex_create("y", chartInstance->c2_d, 0, 0U, 0, 0U, 0),
                false);
  sf_mex_setcell(c2_y, 3, c2_e_y);
  c2_f_y = NULL;
  sf_mex_assign(&c2_f_y, sf_mex_create("y", chartInstance->c2_e, 0, 0U, 0, 0U, 0),
                false);
  sf_mex_setcell(c2_y, 4, c2_f_y);
  sf_mex_assign(&c2_st, c2_y, false);
  return c2_st;
}

static void set_sim_state_c2_Ensamblaje_propio
  (SFc2_Ensamblaje_propioInstanceStruct *chartInstance, const mxArray *c2_st)
{
  const mxArray *c2_b_u;
  chartInstance->c2_doneDoubleBufferReInit = true;
  c2_b_u = sf_mex_dup(c2_st);
  *chartInstance->c2_a = c2_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_b_u, 0)), "a");
  *chartInstance->c2_b = c2_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_b_u, 1)), "b");
  *chartInstance->c2_c = c2_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_b_u, 2)), "c");
  *chartInstance->c2_d = c2_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_b_u, 3)), "d");
  *chartInstance->c2_e = c2_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c2_b_u, 4)), "e");
  sf_mex_destroy(&c2_b_u);
  sf_mex_destroy(&c2_st);
}

static void c2_indexShapeCheck(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

const mxArray *sf_c2_Ensamblaje_propio_get_eml_resolved_functions_info(void)
{
  const mxArray *c2_nameCaptureInfo = NULL;
  c2_nameCaptureInfo = NULL;
  sf_mex_assign(&c2_nameCaptureInfo, sf_mex_create("nameCaptureInfo", NULL, 0,
    0U, 1, 0U, 2, 0, 1), false);
  return c2_nameCaptureInfo;
}

static real_T c2_emlrt_marshallIn(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance, const mxArray *c2_nullptr, const char_T *c2_identifier)
{
  emlrtMsgIdentifier c2_thisId;
  real_T c2_y;
  c2_thisId.fIdentifier = (const char_T *)c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_y = c2_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_nullptr), &c2_thisId);
  sf_mex_destroy(&c2_nullptr);
  return c2_y;
}

static real_T c2_b_emlrt_marshallIn(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId)
{
  real_T c2_b_d;
  real_T c2_y;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_b_u), &c2_b_d, 1, 0, 0U, 0, 0U, 0);
  c2_y = c2_b_d;
  sf_mex_destroy(&c2_b_u);
  return c2_y;
}

static void c2_array_real_T_SetSize(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance, const emlrtStack *c2_sp, c2_coder_array_real_T *c2_coderArray,
  const emlrtRTEInfo *c2_srcLocation, int32_T c2_size0)
{
  real_T *c2_newData;
  int32_T c2_newCapacity;
  int32_T c2_newNumel;
  (void)chartInstance;
  c2_coderArray->size[0] = c2_size0;
  c2_newNumel = (int32_T)emlrtSizeMulR2012b((size_t)1U, (size_t)(uint32_T)
    c2_coderArray->size[0], c2_srcLocation, (void *)c2_sp);
  if (c2_newNumel > c2_coderArray->vector.allocated) {
    c2_newCapacity = c2_coderArray->vector.allocated;
    if (c2_newCapacity < 16) {
      c2_newCapacity = 16;
    }

    while (c2_newCapacity < c2_newNumel) {
      if (c2_newCapacity > 1073741823) {
        c2_newCapacity = MAX_int32_T;
      } else {
        c2_newCapacity <<= 1;
      }
    }

    c2_newData = (real_T *)emlrtMallocMex(sizeof(real_T) * (uint32_T)
      c2_newCapacity);
    if ((void *)c2_newData == NULL) {
      emlrtHeapAllocationErrorR2012b(c2_srcLocation, (void *)c2_sp);
    }

    if ((void *)c2_newData == NULL) {
      emlrtHeapAllocationErrorR2012b(c2_srcLocation, (void *)c2_sp);
    }

    if (c2_coderArray->vector.data != NULL) {
      memcpy(c2_newData, c2_coderArray->vector.data, sizeof(real_T) * (uint32_T)
             c2_coderArray->vector.numel);
      if (c2_coderArray->vector.owner) {
        emlrtFreeMex(c2_coderArray->vector.data);
      }
    }

    c2_coderArray->vector.data = c2_newData;
    c2_coderArray->vector.allocated = c2_newCapacity;
    c2_coderArray->vector.owner = true;
  }

  c2_coderArray->vector.numel = c2_newNumel;
}

static void c2_array_real_T_Constructor(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance, c2_coder_array_real_T *c2_coderArray)
{
  (void)chartInstance;
  c2_coderArray->vector.data = (real_T *)NULL;
  c2_coderArray->vector.numel = 0;
  c2_coderArray->vector.allocated = 0;
  c2_coderArray->vector.owner = true;
  c2_coderArray->size[0] = 0;
}

static void c2_array_real_T_Destructor(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance, c2_coder_array_real_T *c2_coderArray)
{
  (void)chartInstance;
  if (c2_coderArray->vector.owner && (c2_coderArray->vector.data != (real_T *)
       NULL)) {
    emlrtFreeMex(c2_coderArray->vector.data);
  }
}

static uint8_T c2__u8_s8_(SFc2_Ensamblaje_propioInstanceStruct *chartInstance,
  int8_T c2_b_b, int32_T c2_EMLOvCount_src_loc, uint32_T c2_ssid_src_loc,
  int32_T c2_offset_src_loc, int32_T c2_length_src_loc)
{
  uint8_T c2_b_a;
  (void)c2_EMLOvCount_src_loc;
  c2_b_a = (uint8_T)c2_b_b;
  if (c2_b_b < 0) {
    sf_data_overflow_error(chartInstance->S, c2_ssid_src_loc, c2_offset_src_loc,
      c2_length_src_loc);
  }

  return c2_b_a;
}

static void init_dsm_address_info(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static void init_simulink_io_address(SFc2_Ensamblaje_propioInstanceStruct
  *chartInstance)
{
  chartInstance->c2_covrtInstance = (CovrtStateflowInstance *)
    sfrtGetCovrtInstance(chartInstance->S);
  chartInstance->c2_fEmlrtCtx = (void *)sfrtGetEmlrtCtx(chartInstance->S);
  chartInstance->c2_u = (real_T *)ssGetInputPortSignal_wrapper(chartInstance->S,
    0);
  chartInstance->c2_a = (real_T *)ssGetOutputPortSignal_wrapper(chartInstance->S,
    1);
  chartInstance->c2_b = (real_T *)ssGetOutputPortSignal_wrapper(chartInstance->S,
    2);
  chartInstance->c2_c = (real_T *)ssGetOutputPortSignal_wrapper(chartInstance->S,
    3);
  chartInstance->c2_d = (real_T *)ssGetOutputPortSignal_wrapper(chartInstance->S,
    4);
  chartInstance->c2_e = (real_T *)ssGetOutputPortSignal_wrapper(chartInstance->S,
    5);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* SFunction Glue Code */
void sf_c2_Ensamblaje_propio_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(319231762U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(719246308U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(408657023U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(1816323880U);
}

mxArray *sf_c2_Ensamblaje_propio_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

mxArray *sf_c2_Ensamblaje_propio_jit_fallback_info(void)
{
  const char *infoFields[] = { "fallbackType", "fallbackReason",
    "hiddenFallbackType", "hiddenFallbackReason", "incompatibleSymbol" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 5, infoFields);
  mxArray *fallbackType = mxCreateString("late");
  mxArray *fallbackReason = mxCreateString("ir_function_calls");
  mxArray *hiddenFallbackType = mxCreateString("");
  mxArray *hiddenFallbackReason = mxCreateString("");
  mxArray *incompatibleSymbol = mxCreateString("sscanf");
  mxSetField(mxInfo, 0, infoFields[0], fallbackType);
  mxSetField(mxInfo, 0, infoFields[1], fallbackReason);
  mxSetField(mxInfo, 0, infoFields[2], hiddenFallbackType);
  mxSetField(mxInfo, 0, infoFields[3], hiddenFallbackReason);
  mxSetField(mxInfo, 0, infoFields[4], incompatibleSymbol);
  return mxInfo;
}

mxArray *sf_c2_Ensamblaje_propio_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

static const mxArray *sf_get_sim_state_info_c2_Ensamblaje_propio(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  mxArray *mxVarInfo = sf_mex_decode(
    "eNpjYPT0ZQACPiCWYGFgYAPSHEDMxAABrFA+I5TNCBdngYsrAHFJZUEqSLy4KNkzBUjnJeaC+Ym"
    "lFZ55aflg8y0YEOazYTGfEcl8Tqg4BHywp0y/iANIvwGSfhYC+gWArERouMDCZ+DcL0GW+5MGjf"
    "tlyHJ/8qBxvwJZ7k8ZNO5XIsv9qUjuBwC16xo3"
    );
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c2_Ensamblaje_propio_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static const char* sf_get_instance_specialization(void)
{
  return "sFyi9leC8Bh072glHnyMnkE";
}

static void sf_opaque_initialize_c2_Ensamblaje_propio(void *chartInstanceVar)
{
  initialize_params_c2_Ensamblaje_propio((SFc2_Ensamblaje_propioInstanceStruct*)
    chartInstanceVar);
  initialize_c2_Ensamblaje_propio((SFc2_Ensamblaje_propioInstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_enable_c2_Ensamblaje_propio(void *chartInstanceVar)
{
  enable_c2_Ensamblaje_propio((SFc2_Ensamblaje_propioInstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_disable_c2_Ensamblaje_propio(void *chartInstanceVar)
{
  disable_c2_Ensamblaje_propio((SFc2_Ensamblaje_propioInstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_gateway_c2_Ensamblaje_propio(void *chartInstanceVar)
{
  sf_gateway_c2_Ensamblaje_propio((SFc2_Ensamblaje_propioInstanceStruct*)
    chartInstanceVar);
}

static const mxArray* sf_opaque_get_sim_state_c2_Ensamblaje_propio(SimStruct* S)
{
  return get_sim_state_c2_Ensamblaje_propio
    ((SFc2_Ensamblaje_propioInstanceStruct *)sf_get_chart_instance_ptr(S));/* raw sim ctx */
}

static void sf_opaque_set_sim_state_c2_Ensamblaje_propio(SimStruct* S, const
  mxArray *st)
{
  set_sim_state_c2_Ensamblaje_propio((SFc2_Ensamblaje_propioInstanceStruct*)
    sf_get_chart_instance_ptr(S), st);
}

static void sf_opaque_cleanup_runtime_resources_c2_Ensamblaje_propio(void
  *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc2_Ensamblaje_propioInstanceStruct*) chartInstanceVar)->S;
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_Ensamblaje_propio_optimization_info();
    }

    mdl_cleanup_runtime_resources_c2_Ensamblaje_propio
      ((SFc2_Ensamblaje_propioInstanceStruct*) chartInstanceVar);
    utFree(chartInstanceVar);
    if (ssGetUserData(S)!= NULL) {
      sf_free_ChartRunTimeInfo(S);
    }

    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_mdl_start_c2_Ensamblaje_propio(void *chartInstanceVar)
{
  mdl_start_c2_Ensamblaje_propio((SFc2_Ensamblaje_propioInstanceStruct*)
    chartInstanceVar);
  if (chartInstanceVar) {
    sf_reset_warnings_ChartRunTimeInfo(((SFc2_Ensamblaje_propioInstanceStruct*)
      chartInstanceVar)->S);
  }
}

static void sf_opaque_mdl_terminate_c2_Ensamblaje_propio(void *chartInstanceVar)
{
  mdl_terminate_c2_Ensamblaje_propio((SFc2_Ensamblaje_propioInstanceStruct*)
    chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c2_Ensamblaje_propio(SimStruct *S)
{
  mdlProcessParamsCommon(S);
  if (sf_machine_global_initializer_called()) {
    initialize_params_c2_Ensamblaje_propio((SFc2_Ensamblaje_propioInstanceStruct*)
      sf_get_chart_instance_ptr(S));
  }
}

const char* sf_c2_Ensamblaje_propio_get_post_codegen_info(void)
{
  int i;
  const char* encStrCodegen [21] = {
    "eNrdWN2O20QUnqyyK4pKtYVKVKgSFVdwgVR2VVEkBN1NHBppw0YkW1Ruqol9Eg8Zz7jzk2x4EV4",
    "BiRseg0foDY+AxCWXnHGcbJQNiccBbcCS4xzb35xvzpy/Mak0WwSPO3i23yPkAK9v4LlHpsd+Ll",
    "cWzun9Kvk8l9tvEhLGVJmm6Evif4QyggGIju332aUnVtikTRVNdAm9gibwDWjJrWFS+JFnog8KR",
    "IgDpFIZL72aJZYzMWxYETrN+tuYhXEnlpZHpzggjc4Fn/yd3tSaNmqsMwWhaQBEJlbSDuIGp4P1",
    "VlBmXIshHGqbeNtKg+nY1E1Vtyw3LOUQXELYFNpQtILeMN+OoQZq5tLbQ5juzNAySTmjoritY6o",
    "7kKJ3GLhII/w9twatV1Av6usxQY1UjPIg4TXn4QWxbY48W+jW3NvOJjF16NnBgImBs66yCQicP/",
    "pJAVv1a3IEig7gXHjGoJtdcJkt8Nwvi8dg07llqRi0yXRVdClspjcYoYV0Sb2NUNQo59oP25XpG",
    "YyAZ/rr1NAS2Kl+D7DWLOrK51S5KPCMJCvYKws5tiZFxIqv8GgJlSXcrzF5FoCzxLkURGjmOfX5",
    "QJt80mojkxqGUf3srKC+69imMKD6NITCuU9RpgEJZ37lqTdimva4Q6OVTDbLwiOAKA0lum9FfSz",
    "VEG3sm2SvbOUiwQ8N0QDqYCBLGAF693PKbUHOicYM59zjQmPG8tOLWBc/pcAhDWOIXD1hHFqg3Q",
    "C6cH7GOnKCsx0xM6mDDhVLi0aS1RBhIXFW6k5SuBBDIceioWTSyTuCNX4FgFmDKoFl4RTLkpo0k",
    "Hwx1gpedZ1nlWnPEmo47Tnf+AoEVhY3V1cJaYhRFQhs3ZDQNtgO+wFLu9BMG2ynJkEWA1HWj96v",
    "+PWjd3P5eB5LTd1VuFIUy7DATumklzV70GUJZDc6FHuKqZgfTu8jcqX31t56vXv4r1ISR7bEPVv",
    "AVVfY594C7jCXw6OXgdA06XH6PbxMlUyZXMXjdgF7F8GRa7grO8+uHy7gKyv0koXrNuvzdAH31p",
    "Ke6hLuILfZTy9+/OXotxdvv/Nz/fVHd38/3kb/r1U/f76Tyw9m/dm8go2uJfki/vDukj84WTcm7",
    "DMOtSen8aNPjwb8mZi0xDCYxt8GvvtLfGf3H+JpMMFl+VKFzSjfazmZ2ukewI3/ZIHvwQZ73Frw",
    "J0L++HI7/L2ny+tY3YA/xH90yW9vjv/9Uvx7O8P/QSn+4c7wf1iKf7Qz/D8oxR+W+O96nbypuny",
    "TPIvU7UpJ3P6/2Cf8k7ht5+fbj/zX31+XB8jS+4c7PA+yJj/59KW7Nq/XxK9vfD+Xv5h/M6rFjE",
    "crdq35Y9xY9lc9/Z/495+e9pv1yYGzX/7R+7vjE0H5BLep088A+e22ct9b548UUL36W8BN1CGyY",
    "t+zqs7fXopvJ4+ZiORYf/zJ0eOjberaX1tNDC8=",
    ""
  };

  static char newstr [1465] = "";
  newstr[0] = '\0';
  for (i = 0; i < 21; i++) {
    strcat(newstr, encStrCodegen[i]);
  }

  return newstr;
}

static void mdlSetWorkWidths_c2_Ensamblaje_propio(SimStruct *S)
{
  const char* newstr = sf_c2_Ensamblaje_propio_get_post_codegen_info();
  sf_set_work_widths(S, newstr);
  ssSetChecksum0(S,(2962708904U));
  ssSetChecksum1(S,(307879218U));
  ssSetChecksum2(S,(3628378899U));
  ssSetChecksum3(S,(871174441U));
}

static void mdlRTW_c2_Ensamblaje_propio(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlSetupRuntimeResources_c2_Ensamblaje_propio(SimStruct *S)
{
  SFc2_Ensamblaje_propioInstanceStruct *chartInstance;
  chartInstance = (SFc2_Ensamblaje_propioInstanceStruct *)utMalloc(sizeof
    (SFc2_Ensamblaje_propioInstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  memset(chartInstance, 0, sizeof(SFc2_Ensamblaje_propioInstanceStruct));
  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway =
    sf_opaque_gateway_c2_Ensamblaje_propio;
  chartInstance->chartInfo.initializeChart =
    sf_opaque_initialize_c2_Ensamblaje_propio;
  chartInstance->chartInfo.mdlStart = sf_opaque_mdl_start_c2_Ensamblaje_propio;
  chartInstance->chartInfo.mdlTerminate =
    sf_opaque_mdl_terminate_c2_Ensamblaje_propio;
  chartInstance->chartInfo.mdlCleanupRuntimeResources =
    sf_opaque_cleanup_runtime_resources_c2_Ensamblaje_propio;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c2_Ensamblaje_propio;
  chartInstance->chartInfo.disableChart = sf_opaque_disable_c2_Ensamblaje_propio;
  chartInstance->chartInfo.getSimState =
    sf_opaque_get_sim_state_c2_Ensamblaje_propio;
  chartInstance->chartInfo.setSimState =
    sf_opaque_set_sim_state_c2_Ensamblaje_propio;
  chartInstance->chartInfo.getSimStateInfo =
    sf_get_sim_state_info_c2_Ensamblaje_propio;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c2_Ensamblaje_propio;
  chartInstance->chartInfo.mdlSetWorkWidths =
    mdlSetWorkWidths_c2_Ensamblaje_propio;
  chartInstance->chartInfo.extModeExec = NULL;
  chartInstance->chartInfo.restoreLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.restoreBeforeLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.storeCurrentConfiguration = NULL;
  chartInstance->chartInfo.callAtomicSubchartUserFcn = NULL;
  chartInstance->chartInfo.callAtomicSubchartAutoFcn = NULL;
  chartInstance->chartInfo.callAtomicSubchartEventFcn = NULL;
  chartInstance->S = S;
  chartInstance->chartInfo.dispatchToExportedFcn = NULL;
  sf_init_ChartRunTimeInfo(S, &(chartInstance->chartInfo), false, 0);
  init_dsm_address_info(chartInstance);
  init_simulink_io_address(chartInstance);
  if (!sim_mode_is_rtw_gen(S)) {
  }

  mdl_setup_runtime_resources_c2_Ensamblaje_propio(chartInstance);
}

void c2_Ensamblaje_propio_method_dispatcher(SimStruct *S, int_T method, void
  *data)
{
  switch (method) {
   case SS_CALL_MDL_SETUP_RUNTIME_RESOURCES:
    mdlSetupRuntimeResources_c2_Ensamblaje_propio(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c2_Ensamblaje_propio(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c2_Ensamblaje_propio(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c2_Ensamblaje_propio_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
