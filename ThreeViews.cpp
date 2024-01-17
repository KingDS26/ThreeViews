#include "ThreeViews.h"

class myVtkInteractorStyleImage : public vtkInteractorStyleImage
{
public:
    static myVtkInteractorStyleImage* New();
    vtkTypeMacro(myVtkInteractorStyleImage, vtkInteractorStyleImage);

protected:
    vtkImageViewer2* ImageViewer;
    int Slice;
    int MinSlice;
    int MaxSlice;

public:
    void SetImageViewer(vtkImageViewer2* imageViewer)
    {
        this->ImageViewer = imageViewer;
        this->MinSlice = imageViewer->GetSliceMin();
        this->MaxSlice = imageViewer->GetSliceMax();
        this->Slice = (this->MinSlice + this->MaxSlice) / 2;
        this->ImageViewer->SetSlice(this->Slice);
        this->ImageViewer->Render();

        //ImageViewer->GetRenderWindow()->Render();
    }

protected:
    virtual void OnMouseWheelForward()  override
    {
        if (this->Slice < this->MaxSlice)
        {
            this->Slice += 1;
            this->ImageViewer->SetSlice(this->Slice);
            this->ImageViewer->Render();
        }
        //vtkInteractorStyleImage::OnMouseWheelForward();
    }

    virtual void OnMouseWheelBackward()  override
    {
        if (this->Slice > this->MinSlice)
        {
            this->Slice -= 1;
            this->ImageViewer->SetSlice(this->Slice);
            this->ImageViewer->Render();
        }
    }
};
/*
* 宏 作用是为VTK类定义一个标准的创建新对象的方法。
*
* 实现New方法：
*       它为类CustomInteractor实现了一个名为New的静态方法。这个New方法是VTK对象创建的标准方式，它返回一个指向新创建的CustomInteractor对象的指针。
* 内存管理：
*       VTK使用一种称为引用计数的内存管理机制。当使用New方法创建一个对象时，该对象的引用计数设置为1。这意味着当没有更多的引用指向该对象时，该对象将自动被删除。
*       与VTK的工厂机制兼容：vtkStandardNewMacro使得CustomInteractor类与VTK的对象工厂机制兼容。VTK的对象工厂可以在运行时动态替换对象的实例，这对于扩展和定制特定的类行为非常有用。
* 类型安全：
*       使用这个宏创建的New方法提供了一种类型安全的方式来创建新对象，确保返回的是正确类型的实例。
*/
vtkStandardNewMacro(myVtkInteractorStyleImage);


ThreeViews::ThreeViews(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    /*
    * 点击“File”中的“Choose DICOM to Add”
    * 实现槽函数&ThreeViews::chooseDICOMToAdd（）{}功能
    */
    connect(ui.actionChoose_DICOM_to_Add, &QAction::triggered, this, &ThreeViews::chooseDICOMToAdd);

}


/*
* 槽函数chooseDICOMToAdd的实现
* 功能：
*   1.打开文件资源管理器
*   2.DICOM三视可视化
*/
void ThreeViews::chooseDICOMToAdd() {

    // 1.打开文件资源管理器
    QString filePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"));

    // 2.轴向面、矢状面、冠状面可视化
    if (!filePath.isEmpty())
    {
        vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
        reader->SetDirectoryName(filePath.toLocal8Bit().constData());
        reader->Update();
        int* extent = reader->GetOutput()->GetExtent();

        // 为每个 控件 创建一个 vtkImageViewer2 实例
        viewerAxial = vtkSmartPointer<vtkImageViewer2>::New();
        viewerCoronal = vtkSmartPointer<vtkImageViewer2>::New();
        viewerSagittal = vtkSmartPointer<vtkImageViewer2>::New();


        VTKinteractorAxial = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        VTKinteractorAxial->SetRenderWindow(ui.qvtkWidget->GetRenderWindow());
        VTKinteractorAxial->Initialize();

        VTKVTKinteractorCoronal = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        VTKVTKinteractorCoronal->SetRenderWindow(ui.qvtkWidget_2->GetRenderWindow());
        VTKVTKinteractorCoronal->Initialize();

        VTKinteractorSagittal = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        VTKinteractorSagittal->SetRenderWindow(ui.qvtkWidget_3->GetRenderWindow());
        VTKinteractorSagittal->Initialize();

        // 设置轴向(Axial)视图
        viewerAxial->SetInputData(reader->GetOutput());
        viewerAxial->SetupInteractor(VTKinteractorAxial);
        viewerAxial->SetSliceOrientationToXY();
        viewerAxial->SetSlice(extent[5] / 2);
        viewerAxial->SetRenderWindow(ui.qvtkWidget->GetRenderWindow());
        ui.qvtkWidget->show();
        //viewerAxial->SetupInteractor(ui.qvtkWidget->GetRenderWindow()->GetInteractor());
        // 自定义样式应用
        vtkSmartPointer<myVtkInteractorStyleImage> interactorAxial = vtkSmartPointer<myVtkInteractorStyleImage>::New();
        interactorAxial->SetImageViewer(viewerAxial);
        interactorAxial->SetInteractor(ui.qvtkWidget->GetRenderWindow()->GetInteractor());
        VTKinteractorAxial->SetInteractorStyle(interactorAxial);
        viewerAxial->GetRenderer()->ResetCamera();
        viewerAxial->Render();
        ui.qvtkWidget->GetRenderWindow()->Render();

        // 设置冠状(Coronal)视图
        viewerCoronal->SetInputData(reader->GetOutput());
        viewerCoronal->SetSliceOrientationToXZ();
        viewerCoronal->SetupInteractor(VTKVTKinteractorCoronal);
        viewerCoronal->Render();
        viewerCoronal->SetSlice(extent[3] / 2);
        viewerCoronal->SetRenderWindow(ui.qvtkWidget_2->GetRenderWindow());
        ui.qvtkWidget_2->show();
        // viewerCoronal->SetupInteractor(ui.qvtkWidget_2->GetRenderWindow()->GetInteractor());
        vtkSmartPointer<myVtkInteractorStyleImage> interactorCoronal = vtkSmartPointer<myVtkInteractorStyleImage>::New();
        interactorCoronal->SetImageViewer(viewerCoronal);
        VTKVTKinteractorCoronal->GetRenderWindow()->GetInteractor()->SetInteractorStyle(interactorCoronal);
        //VTKVTKinteractorCoronal->SetInteractorStyle(interactorCoronal);
        viewerCoronal->GetRenderer()->ResetCamera();
        viewerCoronal->Render();
        ui.qvtkWidget_2->GetRenderWindow()->Render();

        // 设置矢状(Sagittal)视图
        viewerSagittal->SetInputData(reader->GetOutput());
        viewerSagittal->SetSliceOrientationToYZ();
        viewerSagittal->SetupInteractor(VTKinteractorSagittal);
        viewerSagittal->Render();
        viewerSagittal->SetSlice(extent[1] / 2);
        viewerSagittal->SetRenderWindow(ui.qvtkWidget_3->GetRenderWindow());
        ui.qvtkWidget_3->show();
        // viewerSagittal->SetupInteractor(ui.qvtkWidget_3->GetRenderWindow()->GetInteractor());
        vtkSmartPointer<myVtkInteractorStyleImage> interactorSagittal = vtkSmartPointer<myVtkInteractorStyleImage>::New();
        interactorSagittal->SetImageViewer(viewerSagittal);
        VTKinteractorSagittal->SetInteractorStyle(interactorSagittal);
        viewerSagittal->GetRenderer()->ResetCamera();
        viewerSagittal->Render();
        ui.qvtkWidget_3->GetRenderWindow()->Render();

    }
    else
    {
        std::cout << "can't get the right path!" << std::endl;
    }
}


ThreeViews::~ThreeViews()
{}
