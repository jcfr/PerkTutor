/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// WorkflowSegmentation includes
#include "vtkSlicerWorkflowSegmentationLogic.h"
#include "vtkMRMLWorkflowSegmentationNode.h"

// MRML includes
#include "vtkMRMLViewNode.h"

// VTK includes
#include <vtkNew.h>

// STD includes
#include <cassert>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerWorkflowSegmentationLogic);

//----------------------------------------------------------------------------
vtkSlicerWorkflowSegmentationLogic
::vtkSlicerWorkflowSegmentationLogic()
{
}

//----------------------------------------------------------------------------
vtkSlicerWorkflowSegmentationLogic
::~vtkSlicerWorkflowSegmentationLogic()
{
}



void vtkSlicerWorkflowSegmentationLogic
::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}



void vtkSlicerWorkflowSegmentationLogic
::SetMRMLSceneInternal( vtkMRMLScene* newScene )
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}



void vtkSlicerWorkflowSegmentationLogic
::RegisterNodes()
{
  //assert(this->GetMRMLScene() != 0);  
  if( ! this->GetMRMLScene() )
  {
    return;
  }

  vtkMRMLWorkflowSegmentationNode* wsNode = vtkMRMLWorkflowSegmentationNode::New();
  this->GetMRMLScene()->RegisterNodeClass( wsNode );
  wsNode->Delete();

  vtkMRMLWorkflowProcedureNode* wpNode = vtkMRMLWorkflowProcedureNode::New();
  this->GetMRMLScene()->RegisterNodeClass( wpNode );
  wpNode->Delete();
  
  vtkMRMLWorkflowProcedureStorageNode* wpsNode = vtkMRMLWorkflowProcedureStorageNode::New();
  this->GetMRMLScene()->RegisterNodeClass( wpsNode );
  wpsNode->Delete();

  vtkMRMLWorkflowInputNode* wiNode = vtkMRMLWorkflowInputNode::New();
  this->GetMRMLScene()->RegisterNodeClass( wiNode );
  wiNode->Delete();
  
  vtkMRMLWorkflowInputStorageNode* wisNode = vtkMRMLWorkflowInputStorageNode::New();
  this->GetMRMLScene()->RegisterNodeClass( wisNode );
  wisNode->Delete();

  vtkMRMLWorkflowTrainingNode* wtNode = vtkMRMLWorkflowTrainingNode::New();
  this->GetMRMLScene()->RegisterNodeClass( wtNode );
  wtNode->Delete();
  
  vtkMRMLWorkflowTrainingStorageNode* wtsNode = vtkMRMLWorkflowTrainingStorageNode::New();
  this->GetMRMLScene()->RegisterNodeClass( wtsNode );
  wtsNode->Delete();

  vtkMRMLWorkflowToolNode* toolNode = vtkMRMLWorkflowToolNode::New();
  this->GetMRMLScene()->RegisterNodeClass( toolNode );
  toolNode->Delete();  
}



void vtkSlicerWorkflowSegmentationLogic
::UpdateFromMRMLScene()
{
  assert(this->GetMRMLScene() != 0);
}



void vtkSlicerWorkflowSegmentationLogic
::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
{

}

void vtkSlicerWorkflowSegmentationLogic
::OnMRMLSceneNodeRemoved(vtkMRMLNode* vtkNotUsed(node))
{
	assert(this->GetMRMLScene() != 0);
}


// Workflow Segmentation methods---------------------------------------------------------------------------


vtkMRMLWorkflowToolNode* vtkSlicerWorkflowSegmentationLogic
::GetToolByName( vtkMRMLWorkflowSegmentationNode* workflowNode, std::string toolName )
{
  if ( workflowNode == NULL )
  {
    return NULL;
  }
  
  // Iterate over all tools
  std::vector< std::string > toolIDs = workflowNode->GetToolIDs();  
  for ( int i = 0; i < toolIDs.size(); i++ )
  {
    vtkMRMLWorkflowToolNode* toolNode = vtkMRMLWorkflowToolNode::SafeDownCast( this->GetMRMLScene()->GetNodeByID( toolIDs.at( i ) ) );
    if ( toolNode != NULL && toolNode->GetToolName().compare( toolName ) == 0 )
    {
      return toolNode;
    }
  }
  
  return NULL;
}


void vtkSlicerWorkflowSegmentationLogic
::ResetAllToolBuffers( vtkMRMLWorkflowSegmentationNode* workflowNode )
{
  if ( workflowNode == NULL )
  {
    return;
  }
  
  // Iterate over all tools
  std::vector< std::string > toolIDs = workflowNode->GetToolIDs();  
  for ( int i = 0; i < toolIDs.size(); i++ )
  {
    vtkMRMLWorkflowToolNode* toolNode = vtkMRMLWorkflowToolNode::SafeDownCast( this->GetMRMLScene()->GetNodeByID( toolIDs.at( i ) ) );
    if ( toolNode == NULL || ! toolNode->GetDefined() || ! toolNode->GetInputted() || ! toolNode->GetTrained() )
    {
      continue;
    }
    toolNode->ResetBuffers(); 
  }
  
  workflowNode->Modified();
}


void vtkSlicerWorkflowSegmentationLogic
::TrainAllTools( vtkMRMLWorkflowSegmentationNode* workflowNode, std::vector< std::string > trainingBufferIDs )
{
  if ( workflowNode == NULL )
  {
    return;
  }
  
  // Iterate over all tools
  std::vector< std::string > toolIDs = workflowNode->GetToolIDs();
  
  for ( int i = 0; i < toolIDs.size(); i++ )
  {
    vtkMRMLWorkflowToolNode* toolNode = vtkMRMLWorkflowToolNode::SafeDownCast( this->GetMRMLScene()->GetNodeByID( toolIDs.at( i ) ) );
    if ( toolNode == NULL || ! toolNode->GetDefined() || ! toolNode->GetInputted() || ! toolNode->GetTrained() )
    {
      continue;
    }
    //toolNode->GetWorkflowTrainingNode()->SetName( toolNode->GetWorkflowProcedureNode()->GetName() );
    
    // Grab only the relevant components of the transform buffers
    std::vector< vtkSmartPointer< vtkWorkflowLogRecordBuffer > > trainingRecordBuffers;
    
    for ( int j = 0; j < trainingBufferIDs.size(); j++ )
    {
      vtkMRMLTransformBufferNode* transformBuffer = vtkMRMLTransformBufferNode::SafeDownCast( this->GetMRMLScene()->GetNodeByID( trainingBufferIDs.at( j ) ) );
      vtkSmartPointer< vtkWorkflowLogRecordBuffer > recordBuffer = vtkSmartPointer< vtkWorkflowLogRecordBuffer >::New(); 
      recordBuffer->FromTransformBufferNode( transformBuffer, toolNode->GetWorkflowProcedureNode()->GetProcedureName(), toolNode->GetWorkflowProcedureNode()->GetAllTaskNames() );
      trainingRecordBuffers.push_back( recordBuffer );
    }
    
    toolNode->Train( trainingRecordBuffers );
  }
  
  workflowNode->Modified();
}


bool vtkSlicerWorkflowSegmentationLogic
::GetAllToolsInputted( vtkMRMLWorkflowSegmentationNode* workflowNode )
{
  if ( workflowNode == NULL )
  {
    return false;
  }
  
  std::vector< std::string > toolIDs = workflowNode->GetToolIDs();
  for ( int i = 0; i < toolIDs.size(); i++ )
  {
    vtkMRMLWorkflowToolNode* toolNode = vtkMRMLWorkflowToolNode::SafeDownCast( this->GetMRMLScene()->GetNodeByID( toolIDs.at( i ) ) );
    if ( toolNode == NULL || ! toolNode->GetInputted() )
    {
      return false;
    }    
  }
  
  return true;
}


bool vtkSlicerWorkflowSegmentationLogic
::GetAllToolsTrained( vtkMRMLWorkflowSegmentationNode* workflowNode )
{
  if ( workflowNode == NULL )
  {
    return false;
  }
  
  std::vector< std::string > toolIDs = workflowNode->GetToolIDs();
  for ( int i = 0; i < toolIDs.size(); i++ )
  {
    vtkMRMLWorkflowToolNode* toolNode = vtkMRMLWorkflowToolNode::SafeDownCast( this->GetMRMLScene()->GetNodeByID( toolIDs.at( i ) ) );
    if ( toolNode == NULL || ! toolNode->GetTrained() )
    {
      return false;
    }    
  }
  
  return true;
}


std::vector< std::string > vtkSlicerWorkflowSegmentationLogic
::GetToolStatusStrings( vtkMRMLWorkflowSegmentationNode* workflowNode )
{
  std::vector< std::string > toolStatusStrings;
  if ( workflowNode == NULL )
  {
    return toolStatusStrings;
  }


  std::vector< std::string > toolIDs = workflowNode->GetToolIDs();
  for ( int i = 0; i < toolIDs.size(); i++ )
  {
    vtkMRMLWorkflowToolNode* toolNode = vtkMRMLWorkflowToolNode::SafeDownCast( this->GetMRMLScene()->GetNodeByID( toolIDs.at( i ) ) );    
    if ( toolNode == NULL || ! toolNode->GetDefined() )
    {
      continue;
    }

    std::stringstream toolStatus;
    toolStatus << toolNode->GetName() << " (" << toolNode->GetToolName() << "): ";
    
    if ( toolNode->GetInputted() )
    {
      toolStatus << "Parameters defined, ";
    }
    else
    {
      toolStatus << "No parameters, ";
    }    
    
    if ( toolNode->GetTrained() )
    {
      toolStatus << "trained.";
    }
    else
    {
      toolStatus << "not trained.";
    }
    
    toolStatusStrings.push_back( toolStatus.str() );

  }

  return toolStatusStrings;
}


std::vector< std::string > vtkSlicerWorkflowSegmentationLogic
::GetInstructionStrings( vtkMRMLWorkflowSegmentationNode* workflowNode )
{
  std::vector< std::string > instructionStrings;
  if ( workflowNode == NULL )
  {
    return instructionStrings;
  }

  std::vector< std::string > toolIDs = workflowNode->GetToolIDs();
  for ( int i = 0; i < toolIDs.size(); i++ )
  {
    vtkMRMLWorkflowToolNode* toolNode = vtkMRMLWorkflowToolNode::SafeDownCast( this->GetMRMLScene()->GetNodeByID( toolIDs.at( i ) ) );    
    if ( toolNode == NULL )
    {
      continue;
    }
    vtkWorkflowTask* currentTask = toolNode->GetCurrentTask();
    if ( currentTask == NULL )
    {
      continue;
    }
    
    std::stringstream instruction;
    instruction << toolNode->GetName() << ": ";
    instruction << currentTask->GetName() << " - ";
    instruction << currentTask->GetInstruction() << ".";
    
    instructionStrings.push_back( instruction.str() );

  }

  return instructionStrings;
}



// Node update methods ----------------------------------------------------------

void vtkSlicerWorkflowSegmentationLogic
::SetupRealTimeProcessing( vtkMRMLWorkflowSegmentationNode* wsNode )
{
  // Check conditions
  if ( wsNode == NULL )
  {
    return;
  }

  // Use the python metrics calculator module
  this->ResetAllToolBuffers( wsNode );
}


void vtkSlicerWorkflowSegmentationLogic
::ProcessMRMLNodesEvents( vtkObject* caller, unsigned long event, void* callData )
{
  vtkMRMLWorkflowSegmentationNode* wsNode = vtkMRMLWorkflowSegmentationNode::SafeDownCast( caller );

  // The caller must be a vtkMRMLPerkEvaluatorNode

  // Setup the real-time processing
  if ( wsNode != NULL && event == vtkMRMLWorkflowSegmentationNode::RealTimeProcessingStartedEvent )
  {
    this->SetupRealTimeProcessing( wsNode );
  }

  // Handle an event in the real-time processing
  if ( wsNode != NULL && wsNode->GetRealTimeProcessing() && event == vtkMRMLWorkflowSegmentationNode::TransformRealTimeAddedEvent )
  {
    // The transform name
    std::string* transformName = reinterpret_cast< std::string* >( callData );
    // Convert to a vtkLabelRecord
    vtkSmartPointer< vtkLabelRecord > record = vtkSmartPointer< vtkLabelRecord >::New();
    record->FromTransformRecord( wsNode->GetTransformBufferNode()->GetCurrentTransform( *transformName ), vtkLabelRecord::QUATERNION_RECORD );
    // Get the workflow tool node
    vtkMRMLWorkflowToolNode* toolNode = this->GetToolByName( wsNode, *transformName );
    if ( toolNode != NULL )
    {
      // Get the original task
      vtkWorkflowTask* originalTask = toolNode->GetCurrentTask();
      toolNode->AddAndSegmentRecord( record );
      
      if ( toolNode->GetCurrentTask() != NULL && toolNode->GetCurrentTask() != originalTask )
      {
        vtkSlicerTransformRecorderLogic* trLogic = vtkSlicerTransformRecorderLogic::SafeDownCast( PerkTutorCommon::GetSlicerModuleLogic( "TransformRecorder" ) );
        trLogic->AddMessage( wsNode->GetTransformBufferNode(), toolNode->GetCurrentTask()->GetName(), record->GetTime() );
      }

    }

  }

}


void vtkSlicerWorkflowSegmentationLogic
::ProcessMRMLSceneEvents( vtkObject* caller, unsigned long event, void* callData )
{
  vtkMRMLScene* callerNode = vtkMRMLScene::SafeDownCast( caller );

  // If the added node was a perk evaluator node then observe it
  vtkMRMLNode* addedNode = reinterpret_cast< vtkMRMLNode* >( callData );
  vtkMRMLWorkflowSegmentationNode* wsNode = vtkMRMLWorkflowSegmentationNode::SafeDownCast( addedNode );
  if ( event == vtkMRMLScene::NodeAddedEvent && wsNode != NULL )
  {
    // Observe if a real-time transform event is added
    wsNode->AddObserver( vtkMRMLWorkflowSegmentationNode::TransformRealTimeAddedEvent, ( vtkCommand* ) this->GetMRMLNodesCallbackCommand() );
    wsNode->AddObserver( vtkMRMLWorkflowSegmentationNode::RealTimeProcessingStartedEvent, ( vtkCommand* ) this->GetMRMLNodesCallbackCommand() );
  }

}