static void
<%=c_iter%>(cumo_na_loop_t *const lp)
{
    size_t  i;
    char   *p1, *p2, *p3, *p4;
    ssize_t s1, s2, s3, s4;
    dtype   x, min, max;
    CUMO_INIT_COUNTER(lp, i);
    CUMO_INIT_PTR(lp, 0, p1, s1);
    CUMO_INIT_PTR(lp, 1, p2, s2);
    CUMO_INIT_PTR(lp, 2, p3, s3);
    CUMO_INIT_PTR(lp, 3, p4, s4);
    CUMO_SHOW_SYNCHRONIZE_FIXME_WARNING_ONCE("<%=name%>", "<%=type_name%>");
    cumo_cuda_runtime_check_status(cudaDeviceSynchronize());
    for (; i--;) {
        CUMO_GET_DATA_STRIDE(p1,s1,dtype,x);
        CUMO_GET_DATA_STRIDE(p2,s2,dtype,min);
        CUMO_GET_DATA_STRIDE(p3,s3,dtype,max);
        if (m_gt(min,max)) {rb_raise(cumo_na_eOperationError,"min is greater than max");}
        if (m_lt(x,min)) {x=min;}
        if (m_gt(x,max)) {x=max;}
        CUMO_SET_DATA_STRIDE(p4,s4,dtype,x);
    }
}

static void
<%=c_iter%>_min(cumo_na_loop_t *const lp)
{
    size_t  i;
    char   *p1, *p2, *p3;
    ssize_t s1, s2, s3;
    dtype   x, min;
    CUMO_INIT_COUNTER(lp, i);
    CUMO_INIT_PTR(lp, 0, p1, s1);
    CUMO_INIT_PTR(lp, 1, p2, s2);
    CUMO_INIT_PTR(lp, 2, p3, s3);
    CUMO_SHOW_SYNCHRONIZE_FIXME_WARNING_ONCE("<%=name%>_min", "<%=type_name%>");
    cumo_cuda_runtime_check_status(cudaDeviceSynchronize());
    for (; i--;) {
        CUMO_GET_DATA_STRIDE(p1,s1,dtype,x);
        CUMO_GET_DATA_STRIDE(p2,s2,dtype,min);
        if (m_lt(x,min)) {x=min;}
        CUMO_SET_DATA_STRIDE(p3,s3,dtype,x);
    }
}

static void
<%=c_iter%>_max(cumo_na_loop_t *const lp)
{
    size_t  i;
    char   *p1, *p2, *p3;
    ssize_t s1, s2, s3;
    dtype   x, max;
    CUMO_INIT_COUNTER(lp, i);
    CUMO_INIT_PTR(lp, 0, p1, s1);
    CUMO_INIT_PTR(lp, 1, p2, s2);
    CUMO_INIT_PTR(lp, 2, p3, s3);
    CUMO_SHOW_SYNCHRONIZE_FIXME_WARNING_ONCE("<%=name%>_max", "<%=type_name%>");
    cumo_cuda_runtime_check_status(cudaDeviceSynchronize());
    for (; i--;) {
        CUMO_GET_DATA_STRIDE(p1,s1,dtype,x);
        CUMO_GET_DATA_STRIDE(p2,s2,dtype,max);
        if (m_gt(x,max)) {x=max;}
        CUMO_SET_DATA_STRIDE(p3,s3,dtype,x);
    }
}

/*
  Clip array elements by [min,max].
  If either of min or max is nil, one side is clipped.
  @overload <%=name%>(min,max)
  @param [Cumo::NArray,Numeric] min
  @param [Cumo::NArray,Numeric] max
  @return [Cumo::NArray] result of clip.

  @example
      a = Cumo::Int32.new(10).seq
      p a.clip(1,8)
      # Cumo::Int32#shape=[10]
      # [1, 1, 2, 3, 4, 5, 6, 7, 8, 8]

      p a
      # Cumo::Int32#shape=[10]
      # [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

      p a.inplace.clip(3,6)
      # Cumo::Int32(view)#shape=[10]
      # [3, 3, 3, 3, 4, 5, 6, 6, 6, 6]

      p a
      # Cumo::Int32#shape=[10]
      # [3, 3, 3, 3, 4, 5, 6, 6, 6, 6]

      p a = Cumo::Int32.new(10).seq
      # Cumo::Int32#shape=[10]
      # [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

      p a.clip([3,4,1,1,1,4,4,4,4,4], 8)
      # Cumo::Int32#shape=[10]
      # [3, 4, 2, 3, 4, 5, 6, 7, 8, 8]
*/
static VALUE
<%=c_func(2)%>(VALUE self, VALUE min, VALUE max)
{
    cumo_ndfunc_arg_in_t ain[3] = {{Qnil,0},{cT,0},{cT,0}};
    cumo_ndfunc_arg_out_t aout[1] = {{cT,0}};
    cumo_ndfunc_t ndf_min = { <%=c_iter%>_min, CUMO_STRIDE_LOOP, 2, 1, ain, aout };
    cumo_ndfunc_t ndf_max = { <%=c_iter%>_max, CUMO_STRIDE_LOOP, 2, 1, ain, aout };
    cumo_ndfunc_t ndf_both = { <%=c_iter%>, CUMO_STRIDE_LOOP, 3, 1, ain, aout };

    if (RTEST(min)) {
        if (RTEST(max)) {
            return cumo_na_ndloop(&ndf_both, 3, self, min, max);
        } else {
            return cumo_na_ndloop(&ndf_min, 2, self, min);
        }
    } else {
        if (RTEST(max)) {
            return cumo_na_ndloop(&ndf_max, 2, self, max);
        }
    }
    rb_raise(rb_eArgError,"min and max are not given");
    return Qnil;
}
