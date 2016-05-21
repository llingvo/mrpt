#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <boost/python.hpp>
#include <numpy/arrayobject.h> 
using namespace boost::python;

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
using namespace Eigen;

#include <mrpt/vision/epnp.h>
#include <mrpt/vision/pnp_algos.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
using namespace cv;

class PnPAlgos
{
public:
	PnPAlgos( int new_m );
	~PnPAlgos();
	
	int epnp_solve(PyObject* obj_pts, PyObject* img_pts, int n, PyObject* cam_intrinsic, PyObject* pose_mat);
private:
	int m;
};

PnPAlgos::PnPAlgos( int new_m ){
	m = new_m;
}
PnPAlgos::~PnPAlgos(){
}

template<typename Derived>
int pnpalgo_epnp(MatrixBase<Derived>& obj_pts, MatrixBase<Derived>& img_pts, int n, MatrixBase<Derived>& cam_intrinsic, MatrixBase<Derived>& pose_mat){
	
	MatrixXd cam_in_eig=cam_intrinsic.array().transpose(), img_pts_eig=img_pts.array().transpose(), obj_pts_eig=obj_pts.array().transpose(), R_eig, t_eig, pose_mat_eig=pose_mat; 
	Mat cam_in_cv(3,3,CV_32F), img_pts_cv(n,2,CV_32F), obj_pts_cv(n,3,CV_32F), R_cv, t_cv;
	
	//cout<<"cam_in="<<endl<<cam_in_eig<<endl<<endl;
	//cout<<"obj_pts="<<endl<<obj_pts_eig<<endl<<endl;
	//cout<<"img_pts="<<endl<<img_pts_eig<<endl<<endl;
	
	eigen2cv(cam_in_eig, cam_in_cv);
	eigen2cv(img_pts_eig, img_pts_cv);
	eigen2cv(obj_pts_eig, obj_pts_cv);
	
	//cout<<cam_in_cv<<endl;
	//cout<<img_pts_cv<<endl;
	//cout<<obj_pts_cv<<endl;
	
	epnp e(cam_in_cv, obj_pts_cv, img_pts_cv);
	e.compute_pose(R_cv,t_cv);
	
	//cout<<R_cv<<endl;
	//cout<<t_cv<<endl;
	
	cv2eigen(R_cv, R_eig);
	cv2eigen(t_cv, t_eig);
	
	//cout<<"R_eig="<<endl<<R_eig<<endl<<endl;
	//cout<<"t_eig="<<endl<<t_eig<<endl<<endl;
	
	pose_mat.block(0,0,3,3)=R_eig;
	pose_mat.block(0,3,3,1)=t_eig;
	pose_mat.row(3)<<0,0,0,1;
	
	//cout<<"pose_mat="<<endl<<pose_mat_eig<<endl<<endl;
	
	return 1;
}

int PnPAlgos::epnp_solve(PyObject* obj_pts, PyObject* img_pts, int n, PyObject* cam_intrinsic, PyObject* pose_mat){
	Map<MatrixXd> _obj_pts((double *) PyArray_DATA((PyArrayObject*)obj_pts),3,n);
	Map<MatrixXd> _img_pts((double *) PyArray_DATA((PyArrayObject*)img_pts),n, 2);
	Map<MatrixXd> _pose_mat((double *) PyArray_DATA((PyArrayObject*)pose_mat),4,4);
	Map<MatrixXd> _cam_intrinsic((double *) PyArray_DATA((PyArrayObject*)cam_intrinsic),3,3);
	
	return pnpalgo_epnp(_obj_pts, _img_pts, n, _cam_intrinsic, _pose_mat);
}

void export_pnp()
{
    class_<PnPAlgos>("pnp", init<int>(args("m")))
        .def("epnp_solve", &PnPAlgos::epnp_solve)
    ;
}
