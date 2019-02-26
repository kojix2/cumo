static void
<%=c_iter%>(cumo_na_loop_t *const lp)
{
    size_t i, n;
    size_t i1, n1;
    VALUE  v1, *ptr;
    CUMO_BIT_DIGIT *a1;
    size_t p1;
    size_t s1, *idx1;
    VALUE  x;
    double y;
    CUMO_BIT_DIGIT z;
    size_t len, c;
    double beg, step;

    // TODO(sonots): CUDA kernelize
    CUMO_SHOW_SYNCHRONIZE_FIXME_WARNING_ONCE("<%=name%>", "<%=type_name%>");
    cumo_cuda_runtime_check_status(cudaDeviceSynchronize());

    CUMO_INIT_COUNTER(lp, n);
    CUMO_INIT_PTR_BIT_IDX(lp, 0, a1, p1, s1, idx1);
    v1 = lp->args[1].value;
    i = 0;

    if (lp->args[1].ptr) {
        if (v1 == Qtrue) {
            iter_<%=type_name%>_store_<%=type_name%>(lp);
            i = lp->args[1].shape[0];
            if (idx1) {
                idx1 += i;
            } else {
                p1 += s1 * i;
            }
        }
        goto loop_end;
    }

    ptr = &v1;

    switch(TYPE(v1)) {
    case T_ARRAY:
        n1 = RARRAY_LEN(v1);
        ptr = RARRAY_PTR(v1);
        break;
    case T_NIL:
        n1 = 0;
        break;
    default:
        n1 = 1;
    }

    if (idx1) {
        for (i=i1=0; i1<n1 && i<n; i++,i1++) {
            x = ptr[i1];
#ifdef HAVE_RB_ARITHMETIC_SEQUENCE_EXTRACT
            if (rb_obj_is_kind_of(x, rb_cRange) || rb_obj_is_kind_of(x, rb_cArithSeq)) {
#else
            if (rb_obj_is_kind_of(x, rb_cRange) || rb_obj_is_kind_of(x, rb_cEnumerator)) {
#endif
                cumo_na_step_sequence(x,&len,&beg,&step);
                for (c=0; c<len && i<n; c++,i++) {
                    y = beg + step * c;
                    z = m_from_double(y);
                    CUMO_STORE_BIT(a1, p1+*idx1, z); idx1++;
                }
            }
            if (TYPE(x) != T_ARRAY) {
                if (x == Qnil) x = INT2FIX(0);
                z = m_num_to_data(x);
                CUMO_STORE_BIT(a1, p1+*idx1, z); idx1++;
            }
        }
    } else {
        for (i=i1=0; i1<n1 && i<n; i++,i1++) {
            x = ptr[i1];
#ifdef HAVE_RB_ARITHMETIC_SEQUENCE_EXTRACT
            if (rb_obj_is_kind_of(x, rb_cRange) || rb_obj_is_kind_of(x, rb_cArithSeq)) {
#else
            if (rb_obj_is_kind_of(x, rb_cRange) || rb_obj_is_kind_of(x, rb_cEnumerator)) {
#endif
                cumo_na_step_sequence(x,&len,&beg,&step);
                for (c=0; c<len && i<n; c++,i++) {
                    y = beg + step * c;
                    z = m_from_double(y);
                    CUMO_STORE_BIT(a1, p1, z); p1+=s1;
                }
            }
            if (TYPE(x) != T_ARRAY) {
                z = m_num_to_data(x);
                CUMO_STORE_BIT(a1, p1, z); p1+=s1;
            }
        }
    }

 loop_end:
    z = m_zero;
    if (idx1) {
        for (; i<n; i++) {
            CUMO_STORE_BIT(a1, p1+*idx1, z); idx1++;
        }
    } else {
        for (; i<n; i++) {
            CUMO_STORE_BIT(a1, p1, z); p1+=s1;
        }
    }
}

static VALUE
<%=c_func(:nodef)%>(VALUE self, VALUE rary)
{
    cumo_ndfunc_arg_in_t ain[2] = {{CUMO_OVERWRITE,0}, {rb_cArray,0}};
    cumo_ndfunc_t ndf = {<%=c_iter%>, CUMO_FULL_LOOP, 2, 0, ain, 0};

    cumo_na_ndloop_store_rarray(&ndf, self, rary);
    return self;
}
