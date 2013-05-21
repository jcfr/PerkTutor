
#ifndef VTKWORKFLOWALGORITHM_H
#define VTKWORKFLOWALGORITHM_H

// Standard Includes
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

// VTK includes
#include "vtkObject.h"
#include "vtkObjectBase.h"
#include "vtkObjectFactory.h"
#include "vtkXMLDataElement.h"

// Workflow Segmentation includes
#include "vtkSlicerWorkflowSegmentationModuleLogicExport.h"
#include "vtkRecordBufferRT.h"
#include "vtkMarkovModelRT.h"
#include "vtkWorkflowTool.h"

#include "vtkMRMLTransformBufferNode.h"


// Class representing a particular record for tracking data
class VTK_SLICER_WORKFLOWSEGMENTATION_MODULE_LOGIC_EXPORT
  vtkWorkflowAlgorithm : public vtkObject
{
public:
  vtkTypeMacro(vtkWorkflowAlgorithm,vtkObject);

  static vtkWorkflowAlgorithm *New();

  //vtkWorkflowAlgorithm* DeepCopy();

protected:

  // Constructo/destructor
  vtkWorkflowAlgorithm();
  virtual ~vtkWorkflowAlgorithm();

public:

  // Read training procedures from file
  void AddTrainingBuffer( vtkRecordBuffer* newTrainingProcedure );
  void SegmentBuffer( vtkRecordBuffer* newProcedure );

  // Training and testing phases
  bool Train();
  void AddRecord( vtkLabelRecord* newRecord );
  void AddSegmentRecord( vtkLabelRecord* newRecord );
  void UpdateTask();

  vtkWorkflowTool* Tool;

  std::string GetCurrentTask();
  std::string GetCurrentInstruction();
  std::string GetNextTask();
  std::string GetNextInstruction();
  
  std::vector<double> CalculateTaskProportions();
  std::vector<int> CalculateTaskCentroids();


private:

  // List of procedures for training
  std::vector<vtkRecordBuffer*> TrainingBuffers;

  // The current procedure for real-time segmentation
  vtkRecordBufferRT* BufferRT;
  vtkRecordBufferRT* DerivativeBufferRT;
  vtkRecordBufferRT* FilterBufferRT;
  vtkRecordBufferRT* OrthogonalBufferRT;
  vtkRecordBufferRT* PcaBufferRT;
  vtkRecordBufferRT* CentroidBufferRT;

  vtkMarkovModelRT* MarkovRT;

  int IndexToProcess;
  std::string CurrentTask;
  std::string PrevTask;

};

#endif
