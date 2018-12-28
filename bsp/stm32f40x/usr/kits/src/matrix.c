#include <matrix.h>
#include <stdlib.h>
#include "stdio.h"
#include <rtthread.h> 

static void mat_reset(matrix_t *mat_ptr, uint16_t mode)
{
    uint16_t  i,j;
    for(i=0;i<mat_ptr->row;i++)
    {
        for(j=0;j<mat_ptr->col;j++)
        {
            if(mode == 0)
                mat_ptr->mat[i][j] = 0;
            else
                mat_ptr->mat[i][j] = i*mat_ptr->col + j;
        }
    }
}

static void print_mat(matrix_t *mat_ptr)
{
    uint16_t  i,j;
    for(i=0;i<mat_ptr->row;i++)
    {
        printf("\n");
        for(j=0;j<mat_ptr->col;j++)
            printf(" %6.2f ",mat_ptr->mat[i][j]);
    }
    printf("\n");
}

static void mat_free(matrix_t *mat_ptr)
{
    uint16_t  i;
    for(i=0;i<mat_ptr->row;i++)
    {
        free(mat_ptr->mat[mat_ptr->row - 1- i]);
      
    }
    free(mat_ptr->mat);
}

static int mat_init(matrix_t *mat_ptr, uint16_t row, uint16_t col, uint16_t mode)
{
    int       ret;
    uint16_t  i;
  
    ret = 0;
  
    if((row == 0) || (col == 0))
    {
        ret = -1;
    }
    else
    {
        mat_ptr->row = row;
        mat_ptr->col = col;
        mat_ptr->mat = (float **)malloc(row*sizeof(float *));
        if(mat_ptr->mat == NULL)
            ret = -1;
        else
        {
            for(i=0;i<row;i++)
            {
                *(mat_ptr->mat + i) = (float *)malloc(col*sizeof(float));
                if(mat_ptr->mat[i] == NULL)
                {
                    ret = -1;
                    break;
                }                
            }
            mat_reset(mat_ptr,mode);
        }
    }
    return ret;
}

////////////////////////////////////////////////////////////////////////////
//	a_matrix:转置后的矩阵
//	b_matrix:转置前的矩阵
//	krow    :行数
//	kline   :列数
////////////////////////////////////////////////////////////////////////////

void matrix_trans(matrix_t *dst_mat, const matrix_t *src_mat)
{
    int i,j;
    
    mat_init(dst_mat,src_mat->col,src_mat->row,0);
  
    for (i=0;i<src_mat->row;i++)
    {
        for(j=0;j<src_mat->col;j++)
        {
            dst_mat->mat[j][i] = src_mat->mat[i][j];
        }
    }
}

void matrix_scale(matrix_t *dst_mat, float k)
{
    int i,j;    
  
    for (i=0;i<dst_mat->row;i++)
    {
        for(j=0;j<dst_mat->col;j++)
        {
            dst_mat->mat[i][j] = k * dst_mat->mat[i][j];
        }
    }
}

void matrix_add(matrix_t *dst_mat, const matrix_t *src_mat_a, const matrix_t *src_mat_b)
{
    int i,j;
    
    if((src_mat_a->row != src_mat_b->row) || (src_mat_a->col != src_mat_b->col))
    {
        printf("Error: matrix size mismatch for add operation!\n");
        dst_mat = NULL;
        return;
    }
    else
    {
        mat_init(dst_mat,src_mat_a->row,src_mat_a->col,0);
      
        for (i=0;i<dst_mat->row;i++)
        {
            for(j=0;j<dst_mat->col;j++)
            {
                dst_mat->mat[i][j] = src_mat_a->mat[i][j] + src_mat_b->mat[i][j];
            }
        }
    }
}

void matrix_mult(matrix_t *dst_mat, const matrix_t *src_mat_a, const matrix_t *src_mat_b)
{
    int i,j,k;    
  
    if(src_mat_a->col != src_mat_b->row)
    {
        printf("Error: matrix size mismatch for mult operation!\n");
        dst_mat = NULL;
        return;
    }
    else
    {
        mat_init(dst_mat,src_mat_a->row,src_mat_b->col,0);
      
        for (i=0;i<src_mat_a->row;i++)
        {
            for(j=0;j<src_mat_b->col;j++)
            {
                for(k=0;k<src_mat_a->col;k++)
                    dst_mat->mat[i][j] += src_mat_a->mat[i][k] * src_mat_b->mat[k][j];
            }
        }
    }
}

static void mat_test(uint16_t row,uint16_t col,uint16_t mode)
{
    matrix_t mat,mat_T,mat_mul,mat_a;
  
    mat_init(&mat,row,col,mode);
    print_mat(&mat);
  
    matrix_trans(&mat_T,&mat);
    print_mat(&mat_T);
  
    matrix_mult(&mat_mul,&mat,&mat_T);
    print_mat(&mat_mul);   
  
    matrix_add(&mat_a,&mat,&mat);
    print_mat(&mat_a); 
  
    mat_free(&mat_a);
    mat_free(&mat_mul);
    mat_free(&mat_T);
    mat_free(&mat);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(mat_test, system reset.);
#endif
