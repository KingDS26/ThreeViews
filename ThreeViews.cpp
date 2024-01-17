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
* �� ������ΪVTK�ඨ��һ����׼�Ĵ����¶���ķ�����
*
* ʵ��New������
*       ��Ϊ��CustomInteractorʵ����һ����ΪNew�ľ�̬���������New������VTK���󴴽��ı�׼��ʽ��������һ��ָ���´�����CustomInteractor�����ָ�롣
* �ڴ����
*       VTKʹ��һ�ֳ�Ϊ���ü������ڴ������ơ���ʹ��New��������һ������ʱ���ö�������ü�������Ϊ1������ζ�ŵ�û�и��������ָ��ö���ʱ���ö����Զ���ɾ����
*       ��VTK�Ĺ������Ƽ��ݣ�vtkStandardNewMacroʹ��CustomInteractor����VTK�Ķ��󹤳����Ƽ��ݡ�VTK�Ķ��󹤳�����������ʱ��̬�滻�����ʵ�����������չ�Ͷ����ض�������Ϊ�ǳ����á�
* ���Ͱ�ȫ��
*       ʹ������괴����New�����ṩ��һ�����Ͱ�ȫ�ķ�ʽ�������¶���ȷ�����ص�����ȷ���͵�ʵ����
*/
vtkStandardNewMacro(myVtkInteractorStyleImage);


ThreeViews::ThreeViews(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    /*
    * �����File���еġ�Choose DICOM to Add��
    * ʵ�ֲۺ���&ThreeViews::chooseDICOMToAdd����{}����
    */
    connect(ui.actionChoose_DICOM_to_Add, &QAction::triggered, this, &ThreeViews::chooseDICOMToAdd);

}


/*
* �ۺ���chooseDICOMToAdd��ʵ��
* ���ܣ�
*   1.���ļ���Դ������
*   2.DICOM���ӿ��ӻ�
*/
void ThreeViews::chooseDICOMToAdd() {

    // 1.���ļ���Դ������
    QString filePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"));

    // 2.�����桢ʸ״�桢��״����ӻ�
    if (!filePath.isEmpty())
    {
        vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
        reader->SetDirectoryName(filePath.toLocal8Bit().constData());
        reader->Update();
        int* extent = reader->GetOutput()->GetExtent();

        // Ϊÿ�� �ؼ� ����һ�� vtkImageViewer2 ʵ��
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

        // ��������(Axial)��ͼ
        viewerAxial->SetInputData(reader->GetOutput());
        viewerAxial->SetupInteractor(VTKinteractorAxial);
        viewerAxial->SetSliceOrientationToXY();
        viewerAxial->SetSlice(extent[5] / 2);
        viewerAxial->SetRenderWindow(ui.qvtkWidget->GetRenderWindow());
        ui.qvtkWidget->show();
        //viewerAxial->SetupInteractor(ui.qvtkWidget->GetRenderWindow()->GetInteractor());
        // �Զ�����ʽӦ��
        vtkSmartPointer<myVtkInteractorStyleImage> interactorAxial = vtkSmartPointer<myVtkInteractorStyleImage>::New();
        interactorAxial->SetImageViewer(viewerAxial);
        interactorAxial->SetInteractor(ui.qvtkWidget->GetRenderWindow()->GetInteractor());
        VTKinteractorAxial->SetInteractorStyle(interactorAxial);
        viewerAxial->GetRenderer()->ResetCamera();
        viewerAxial->Render();
        ui.qvtkWidget->GetRenderWindow()->Render();

        // ���ù�״(Coronal)��ͼ
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

        // ����ʸ״(Sagittal)��ͼ
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
