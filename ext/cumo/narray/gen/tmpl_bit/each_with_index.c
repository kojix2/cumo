static inline void
yield_each_with_index(dtype x, size_t *c, VALUE *a, int nd, int md)
{
    int j;

    a[0] = m_data_to_num(x);
    for (j=0; j<=nd; j++) {
        a[j+1] = SIZET2NUM(c[j]);
    }
    rb_yield(rb_ary_new4(md,a));
}


static void
<%=c_iter%>(cumo_na_loop_t *const lp)
{
    size_t   i;
    CUMO_BIT_DIGIT *a1, x=0;
    size_t   p1;
    ssize_t  s1;
    size_t  *idx1;

    VALUE *a;
    size_t *c;
    int nd, md;

    c = (size_t*)(lp->opt_ptr);
    nd = lp->ndim - 1;
    md = lp->ndim + 1;
    a = ALLOCA_N(VALUE,md);

    CUMO_INIT_COUNTER(lp, i);
    CUMO_INIT_PTR_BIT_IDX(lp, 0, a1, p1, s1, idx1);
    c[nd] = 0;

    CUMO_SHOW_SYNCHRONIZE_WARNING_ONCE("<%=name%>", "<%=type_name%>");

    if (idx1) {
        for (; i--;) {
            cumo_cuda_runtime_check_status(cudaDeviceSynchronize());
            CUMO_LOAD_BIT(a1, p1+*idx1, x); idx1++;
            yield_each_with_index(x,c,a,nd,md);
            c[nd]++;
        }
    } else {
        for (; i--;) {
            cumo_cuda_runtime_check_status(cudaDeviceSynchronize());
            CUMO_LOAD_BIT(a1, p1, x); p1+=s1;
            yield_each_with_index(x,c,a,nd,md);
            c[nd]++;
        }
    }
}

/*
  Invokes the given block once for each element of self,
  passing that element and indices along each axis as parameters.
  @overload <%=name%>
  @return [Cumo::NArray] self
  For a block {|x,i,j,...| ... }
  @yield [x,i,j,...]  x is an element, i,j,... are multidimensional indices.
*/
static VALUE
<%=c_func(0)%>(VALUE self)
{
    cumo_ndfunc_arg_in_t ain[1] = {{Qnil,0}};
    cumo_ndfunc_t ndf = {<%=c_iter%>, CUMO_FULL_LOOP_NIP, 1,0, ain,0};

    cumo_na_ndloop_with_index(&ndf, 1, self);
    return self;
}
