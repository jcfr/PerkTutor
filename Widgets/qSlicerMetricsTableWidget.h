/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qSlicerMetricsTableWidget_h
#define __qSlicerMetricsTableWidget_h

// Qt includes
#include "qSlicerWidget.h"

// VTK includes
#include <vtkTable.h>

#include "vtkSlicerModuleLogic.h"
#include "vtkMRMLTableNode.h"
#include "vtkSlicerPerkEvaluatorLogic.h"

// FooBar Widgets includes
#include "qSlicerPerkEvaluatorModuleWidgetsExport.h"
#include "ui_qSlicerMetricsTableWidget.h"

#include "PerkTutorCommon.h"

class qSlicerMetricsTableWidgetPrivate;

/// \ingroup Slicer_QtModules_CreateModels
class Q_SLICER_MODULE_PERKEVALUATOR_WIDGETS_EXPORT 
qSlicerMetricsTableWidget : public qSlicerWidget
{
  Q_OBJECT
public:
  typedef qSlicerWidget Superclass;
  qSlicerMetricsTableWidget(QWidget *parent=0);
  virtual ~qSlicerMetricsTableWidget();

  virtual void setMetricsTableNode( vtkMRMLNode* newMetricsTableNode );
  virtual vtkMRMLTableNode* getMetricsTableNode();
  virtual vtkMRMLTableNode* addMetricsTableNode();

protected slots:

  virtual void onMetricsTableNodeChanged( vtkMRMLNode* newMetricsTableNode );
  void onMetricsTableNodeModified();
  
  void onClipboardButtonClicked();

  void updateWidget();

signals:

  void metricsTableNodeChanged( vtkMRMLNode* newMetricsTableNode );
  void metricsTableNodeModified();

protected:

  QScopedPointer<qSlicerMetricsTableWidgetPrivate> d_ptr;

  vtkMRMLTableNode* MetricsTableNode;
  vtkSlicerPerkEvaluatorLogic* PerkEvaluatorLogic;

  virtual void setup();

private:
  Q_DECLARE_PRIVATE(qSlicerMetricsTableWidget);
  Q_DISABLE_COPY(qSlicerMetricsTableWidget);

};

#endif
