#include <rtthread.h>

#define MAX_IND 8

static void expansion(const int32_t* src_mat, int32_t * dst_mat,int x_size, int y_size, int x_exp, int y_exp)
{
    int i,j;
    int dst_xsize,dst_ysize,dst_len;    
    
    dst_xsize = x_size * x_exp - (x_exp - 1) ;
    dst_ysize = y_size * y_exp - (y_exp - 1) ;

    dst_len = dst_xsize * dst_ysize;
    
    //initialize destination 2-D matrix
    for(i=0;i<dst_len;i++)
        *(dst_mat+i) = 0;
  
    for(i=0;i<y_size;i++)   // rows iteration
    {
        for(j=0;j<x_size;j++) //columns iteration
        {
            *(dst_mat + i*dst_xsize*y_exp + j*x_exp) = *(src_mat + i*x_size + j); 
        }
    }    
} 

static void int_calc(int32_t* data_ptr, int x_size, int y_size, int axis)
{
    int i,j,k,step,delta,d_size,x_scope;
    int ind_buf[MAX_IND];
    
    j = 0;
    step = 0;
  
    //initialize index buf
    for(i=0;i<MAX_IND;i++)
        ind_buf[i] = 0;
  
    if(axis == 0)
    {
        d_size = x_size;
        x_scope = 1;
    }
    else
    {
        d_size = y_size;
        x_scope = x_size;
    }
  
    //store none-zero variable into ind_buf
    for(i=0;i<d_size;i++)
    {
        if(*(data_ptr+i*x_scope) != 0)
        {
            ind_buf[j] = i;
            j++;
        }
    }
    
    //calculate vertical/horizontal interpolation
    if(j>=2)
    {
        for(i=0;i<j;i++)
        {
            step = (ind_buf[i+1] - ind_buf[i]);
            delta = (*(data_ptr+ind_buf[i+1]*x_scope) - *(data_ptr+ind_buf[i]*x_scope))/step;
            for(k=1;k<step;k++)
                *(data_ptr+(ind_buf[i]+k)*x_scope) = *(data_ptr+ind_buf[i]*x_scope) + k*delta;
        }      
    }
    else
    {
//        rt_kprintf("not enough beacons,axis = %d\n",axis);
    }
}

static void cal_mat(int32_t* data_ptr, int x_size, int y_size,int axis)
{
    int i;    
    
    if(axis == 0)
    {
        for(i=0;i<y_size;i++)
        {
            int_calc((data_ptr+i*x_size),x_size,y_size,0);
        }
        
        for(i=0;i<x_size;i++)
        {
            int_calc((data_ptr+i),x_size,y_size,1);
        }    
    }
    else
    {
        for(i=0;i<x_size;i++)
        {
            int_calc((data_ptr+i),x_size,y_size,1);
        } 
        
        for(i=0;i<y_size;i++)
        {
            int_calc((data_ptr+i*x_size),x_size,y_size,0);
        }
    }
}

static void bilin_calc(int32_t* data_ptr, int32_t* buf_ptr, int x_size, int y_size)
{
    int total_cnt,i;
    cal_mat(data_ptr,x_size,y_size,0);
    cal_mat(buf_ptr,x_size,y_size,1);
    total_cnt = x_size*y_size;
    for(i=0;i<total_cnt;i++)
    {
        *(data_ptr+i) = (*(data_ptr+i) + *(buf_ptr+i))/2;
    }
}

static void print_mat(const int32_t* src_buf, int x, int y)
{
    int i,j;
    for(i=0;i<y;i++)
    {
        for(j=0;j<x;j++)
            rt_kprintf(" %3d ", *(src_buf+i*x+j));
        rt_kprintf("\n");
    }
    rt_kprintf("\n");
}

//static void print_mat_ex(const int32_t* src_buf, int x, int y)
//{
//    int i,j;
//    j = x*y;
//    for(i=0;i<j;i++)
//    {
//        rt_kprintf(" %3d ", *(src_buf+i));
//    }
//    rt_kprintf("\n");
//}



static void  int_test(int x_exp, int y_exp)
{
    int x_size,y_size;
    int dst_xsize,dst_ysize; 
  
    const int32_t arr_test[12] = {1,50,20,40,25,66,21,38,9,22,1,203};
    int32_t int_buf[256];
    int32_t int_buf_d[256];  
      
    x_size = 3;
    y_size = 4;  

    dst_xsize = x_size * x_exp - (x_exp - 1) ;
    dst_ysize = y_size * y_exp - (y_exp - 1) ;
  
    rt_kprintf("src\n");
    print_mat(arr_test,x_size,y_size);
 
    expansion(arr_test,int_buf,x_size,y_size,x_exp,y_exp);  
    rt_kprintf("dest, %d x %d\n",dst_xsize,dst_ysize);
    print_mat(int_buf,dst_xsize,dst_ysize);
//    print_mat_ex(int_buf,dst_xsize,dst_ysize);
    
    rt_kprintf("intp_mat, axis= 0\n");
    cal_mat(int_buf,dst_xsize,dst_ysize,0);
    print_mat(int_buf,dst_xsize,dst_ysize);  
    
    expansion(arr_test,int_buf,x_size,y_size,x_exp,y_exp);
    rt_kprintf("intp_mat, axis= 1\n");
    cal_mat(int_buf,dst_xsize,dst_ysize,1);
    print_mat(int_buf,dst_xsize,dst_ysize);      
    
    expansion(arr_test,int_buf,x_size,y_size,x_exp,y_exp);
    expansion(arr_test,int_buf_d,x_size,y_size,x_exp,y_exp); 
    rt_kprintf("bilin_mat\n");
    bilin_calc(int_buf,int_buf_d,dst_xsize,dst_ysize);
    print_mat(int_buf,dst_xsize,dst_ysize);  
}

#ifdef RT_USING_FINSH
#include <finsh.h>
//FINSH_FUNCTION_EXPORT(int_test, system reset.);
FINSH_FUNCTION_EXPORT(int_test, system reset.);
#endif
