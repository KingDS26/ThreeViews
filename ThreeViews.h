#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ThreeViews.h"
#include <vtkSmartPointer.h>
#include <QAction>
#include <QFileDialog>
#include <QPalette>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindow.h>
#include <vtkInteractorStyleImage.h>
#include <vtkObjectFactory.h>
#include <vtkRenderer.h>  
#include <vtkRenderWindow.h>  
#include <vtkRenderer.h>  
#include <vtkImageData.h>  

#include <vtkRenderWindowInteractor.h>  
class ThreeViews : public QMainWindow
{
	Q_OBJECT

public:
	ThreeViews(QWidget* parent = nullptr);
	~ThreeViews();

public slots:
	void chooseDICOMToAdd();

private:
	Ui::ThreeViewsClass ui;
	//vtkSmartPointer<QVTKOpenGLNativeWidget> QVTKOpenGLNativeWidgetX;
	//vtkSmartPointer<QVTKOpenGLNativeWidget> QVTKOpenGLNativeWidgetY;
	//vtkSmartPointer<QVTKOpenGLNativeWidget> QVTKOpenGLNativeWidgetZ;
	//vtkSmartPointer<QVTKOpenGLNativeWidget> QVTKOpenGLNativeWidgetV;

	// 声明交互器变量
	vtkSmartPointer<vtkRenderWindowInteractor> VTKinteractorAxial;
	vtkSmartPointer<vtkRenderWindowInteractor> VTKVTKinteractorCoronal;
	vtkSmartPointer<vtkRenderWindowInteractor> VTKinteractorSagittal;

	// 声明imageViewer2变量
	vtkSmartPointer<vtkImageViewer2> viewerAxial;
	vtkSmartPointer<vtkImageViewer2> viewerCoronal;
	vtkSmartPointer<vtkImageViewer2> viewerSagittal;
};
