
#include "vtkWorkflowToolCollection.h"

vtkStandardNewMacro( vtkWorkflowToolCollection );


vtkWorkflowToolCollection
::vtkWorkflowToolCollection()
{
// Nothing to do since we have a vector
  this->minTime = 0;
  this->maxTime = 0;
}


vtkWorkflowToolCollection
::~vtkWorkflowToolCollection()
{
  this->tools.clear();
}


vtkWorkflowToolCollection* vtkWorkflowToolCollection
::DeepCopy()
{
  vtkWorkflowToolCollection* newWorkflowToolCollection = vtkWorkflowToolCollection::New();
  for ( int i = 0; i < this->GetNumTools(); i++ )
  {
    newWorkflowToolCollection->tools.push_back( this->GetToolAt(i)->DeepCopy() );
  }

  return newWorkflowToolCollection;
}


int vtkWorkflowToolCollection
::GetNumTools()
{
  return this->tools.size();
}


vtkWorkflowTool* vtkWorkflowToolCollection
::GetToolAt( int index )
{
  return this->tools.at(index);
}


vtkWorkflowTool* vtkWorkflowToolCollection
::GetToolByName( std::string name )
{
  for ( int i = 0; i < this->GetNumTools(); i++ )
  {
	if ( strcmp( name.c_str(), this->GetToolAt(i)->Name.c_str() ) == 0 )
	{
	  return this->GetToolAt(i);
    }
  }

  vtkWorkflowTool* tool = vtkWorkflowTool::New();
  return tool;
}


bool vtkWorkflowToolCollection
::GetDefined()
{
  for ( int i = 0; i < this->GetNumTools(); i++ )
  {
    if ( ! this->GetToolAt(i)->Defined )
	{
      return false;
	}
  }

  return true;
}


bool vtkWorkflowToolCollection
::GetInputted()
{
  for ( int i = 0; i < this->GetNumTools(); i++ )
  {
    if ( ! this->GetToolAt(i)->Inputted )
	{
      return false;
	}
  }

  return true;
}


bool vtkWorkflowToolCollection
::GetTrained()
{
  for ( int i = 0; i < this->GetNumTools(); i++ )
  {
    if ( ! this->GetToolAt(i)->Trained )
	{
      return false;
	}
  }

  return true;
}


double vtkWorkflowToolCollection
::GetMinTime()
{
  double minTime = this->GetToolAt(0)->Buffer->GetRecordAt(0)->GetTime(); 
  for ( int i = 0; i < this->GetNumTools(); i++ )
  {
    if ( this->GetToolAt(i)->Buffer->GetRecordAt(0)->GetTime() < minTime )
	{
      minTime = this->GetToolAt(i)->Buffer->GetRecordAt(0)->GetTime();
	}
  }
  return minTime;
}


double vtkWorkflowToolCollection
::GetMaxTime()
{
  double maxTime = this->GetToolAt(0)->Buffer->GetCurrentRecord()->GetTime(); 
  for ( int i = 0; i < this->GetNumTools(); i++ )
  {
    if ( this->GetToolAt(i)->Buffer->GetCurrentRecord()->GetTime() > maxTime )
	{
      maxTime = this->GetToolAt(i)->Buffer->GetCurrentRecord()->GetTime();
	}
  }
  return maxTime;
}


double vtkWorkflowToolCollection
::GetTotalTime()
{
  return ( this->GetMaxTime() - this->GetMinTime() );
}


std::string vtkWorkflowToolCollection
::ProcedureToXMLString()
{
  std::stringstream xmlstring;

  xmlstring << "<PerkProcedure>" << std::endl;
  for ( int i = 0; i < this->GetNumTools(); i++ )
  {
    xmlstring << this->GetToolAt(i)->ProcedureToXMLString();
  }
  xmlstring << "</PerkProcedure>" << std::endl;

  return xmlstring.str();
}


void vtkWorkflowToolCollection
::ProcedureFromXMLElement( vtkXMLDataElement* element )
{

  if ( ! element || strcmp( element->GetName(), "PerkProcedure" ) != 0 )
  {
    return;
  }

  int numElements = element->GetNumberOfNestedElements();

  for ( int i = 0; i < numElements; i++ )
  {

    vtkXMLDataElement* noteElement = element->GetNestedElement( i );
    if ( strcmp( noteElement->GetName(), "Tool" ) != 0 )
    {
      continue;  // If it's not a "Tool", jump to the next.
    }

	this->GetToolByName( std::string( noteElement->GetAttribute( "Name" ) ) )->ProcedureFromXMLElement( noteElement );

  }

}


std::string vtkWorkflowToolCollection
::InputToXMLString()
{
  std::stringstream xmlstring;

  xmlstring << "<WorkflowSegmentationParameters>" << std::endl;
  for ( int i = 0; i < this->GetNumTools(); i++ )
  {
    xmlstring << this->GetToolAt(i)->InputToXMLString();
  }
  xmlstring << "</WorkflowSegmentationParameters>" << std::endl;

  return xmlstring.str();
}


void vtkWorkflowToolCollection
::InputFromXMLElement( vtkXMLDataElement* element )
{

  if ( ! element || strcmp( element->GetName(), "WorkflowSegmentationParameters" ) != 0 || ! this->GetDefined() )
  {
    return;
  }

  int numElements = element->GetNumberOfNestedElements();

  for ( int i = 0; i < numElements; i++ )
  {

    vtkXMLDataElement* noteElement = element->GetNestedElement( i );
    if ( strcmp( noteElement->GetName(), "Tool" ) != 0 )
    {
      continue;  // If it's not a "Tool", jump to the next.
    }

	this->GetToolByName( std::string( noteElement->GetAttribute( "Name" ) ) )->InputFromXMLElement( noteElement );

  }

}


std::string vtkWorkflowToolCollection
::TrainingToXMLString()
{
  std::stringstream xmlstring;

  xmlstring << "<WorkflowSegmentationParameters>" << std::endl;
  for ( int i = 0; i < this->GetNumTools(); i++ )
  {
    xmlstring << this->GetToolAt(i)->TrainingToXMLString();
  }
  xmlstring << "</WorkflowSegmentationParameters>" << std::endl;

  return xmlstring.str();
}


void vtkWorkflowToolCollection
::TrainingFromXMLElement( vtkXMLDataElement* element )
{

  if ( ! element || strcmp( element->GetName(), "WorkflowSegmentationParameters" ) != 0 || ! this->GetInputted() )
  {
    return;
  }

  int numElements = element->GetNumberOfNestedElements();

  for ( int i = 0; i < numElements; i++ )
  {

    vtkXMLDataElement* noteElement = element->GetNestedElement( i );
    if ( strcmp( noteElement->GetName(), "Tool" ) != 0 )
    {
      continue;  // If it's not a "Tool", jump to the next.
    }

	this->GetToolByName( std::string( noteElement->GetAttribute( "Name" ) ) )->TrainingFromXMLElement( noteElement );

  }

}


std::string vtkWorkflowToolCollection
::BuffersToXMLString()
{
  std::stringstream xmlstring;

  // Create a new vtkMRMLTransformBufferNode and then write its xml string
  vtkMRMLTransformBufferNode* transformBufferNode = vtkMRMLTransformBufferNode::New();

  for ( int i = 0; i < this->GetNumTools(); i++ )
  {
    vtkMRMLTransformBufferNode* tempTransformBufferNode = this->GetToolAt(i)->Buffer->ToTransformBufferNode();
    for ( int j = 0; j < tempTransformBufferNode->GetNumTransforms(); j++ )
	{      
      transformBufferNode->AddTransform( tempTransformBufferNode->GetTransformAt(j) );
	}
	for ( int j = 0; j < tempTransformBufferNode->GetNumMessages(); j++ )
	{      
      transformBufferNode->AddMessage( tempTransformBufferNode->GetMessageAt(j) );
	}
  }

  return transformBufferNode->ToXMLString();
}


void vtkWorkflowToolCollection
::BuffersFromXMLElement( vtkXMLDataElement* element )
{

  if ( strcmp( element->GetName(), "TransformRecorderLog" ) != 0 )
  {
    return;
  }

  // For this one, check all tools each of which will handle all elements
  vtkMRMLTransformBufferNode* transformBufferNode = vtkMRMLTransformBufferNode::New();
  transformBufferNode->FromXMLElement( element );
  std::vector<vtkMRMLTransformBufferNode*> transformBufferNodeVector = transformBufferNode->SplitBufferByName();
  for ( int i = 0; i < transformBufferNodeVector.size(); i++ )
  {
    vtkWorkflowTool* currentTool = this->GetToolByName( transformBufferNodeVector.at(i)->GetCurrentTransform()->GetDeviceName() );
	vtkRecordBuffer* currentBuffer = vtkRecordBuffer::New();
	currentBuffer->FromTransformBufferNode( transformBufferNodeVector.at(i) );
    currentTool->Buffer = currentBuffer;
  }

}