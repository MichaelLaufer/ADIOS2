/*
 * SingleBP.cpp
 *
 *  Created on: Dec 19, 2016
 *      Author: wfg
 */

#include <iostream>

#include "engine/writer/Writer.h"
#include "engine/writer/WriterTemplates.h"
#include "core/Support.h"
#include "functions/adiosFunctions.h" //GetTotalSize

//supported capsules
#include "capsule/Heap.h"

//supported transports
#include "transport/POSIX.h"
#include "transport/FStream.h"
#include "transport/File.h"


namespace adios
{


Writer::Writer( const std::string streamName, const std::string accessMode, const MPI_Comm mpiComm,
                const Method& method, const bool debugMode, const unsigned int cores ):
    Engine( "Writer", streamName, accessMode, mpiComm, method, debugMode, cores, " Writer constructor (or call to ADIOS Open).\n" ),
    m_Buffer{ Heap( accessMode, m_RankMPI, m_DebugMode ) },
    m_MaxBufferSize{ m_Buffer.m_Data.max_size() },
    m_BP1Writer{ format::BP1Writer( 1 ) }
{
    Init( );
}


Writer::~Writer( )
{ }


void Writer::Init( )
{
    auto itGrowthFactor = m_Method.m_Parameters.find( "buffer_growth" );
    if( itGrowthFactor != m_Method.m_Parameters.end() )
    {
        m_GrowthFactor = std::stof( itGrowthFactor->second ); //float
        m_BP1Writer.m_GrowthFactor = m_GrowthFactor;
    }

    auto itMaxBufferSize = m_Method.m_Parameters.find( "max_size_MB" );
    if( itMaxBufferSize != m_Method.m_Parameters.end() )
        m_MaxBufferSize = std::stoul( itGrowthFactor->second ) * 1000000; //convert to bytes

    auto itVerbosity = m_Method.m_Parameters.find( "verbose" );
    if( itVerbosity != m_Method.m_Parameters.end() )
        m_BP1Writer.m_Verbosity = std::stoi( itVerbosity->second );

    InitTransports( );
}


void Writer::Write( Group& group, const std::string variableName, const char* values )
{
    auto index = PreSetVariable( group, variableName, " from call to Write char*" );
    Variable<char>& variable = group.m_Char[index]; //must be a reference
    variable.Values = values;
    WriterWriteVariable( group, variableName, variable, m_GrowthFactor, m_MaxBufferSize, m_RankMPI, m_Buffer, m_Transports, m_BP1Writer, m_Cores );
}

void Writer::Write( Group& group, const std::string variableName, const unsigned char* values )
{
    auto index = PreSetVariable( group, variableName, " from call to Write unsigned char*" );
    Variable<unsigned char>& variable = group.m_UChar[index]; //must be a reference
    variable.Values = values;
    WriterWriteVariable( group, variableName, variable, m_GrowthFactor, m_MaxBufferSize, m_RankMPI, m_Buffer, m_Transports, m_BP1Writer, m_Cores );
}

void Writer::Write( Group& group, const std::string variableName, const short* values )
{
    auto index = PreSetVariable( group, variableName, " from call to Write short*" );
    Variable<short>& variable = group.m_Short[index]; //must be a reference
    variable.Values = values;
    WriterWriteVariable( group, variableName, variable, m_GrowthFactor, m_MaxBufferSize, m_RankMPI, m_Buffer, m_Transports, m_BP1Writer, m_Cores );
}

void Writer::Write( Group& group, const std::string variableName, const unsigned short* values )
{
    auto index = PreSetVariable( group, variableName, " from call to Write unsigned short*" );
    Variable<unsigned short>& variable = group.m_UShort[index]; //must be a reference
    variable.Values = values;
    WriterWriteVariable( group, variableName, variable, m_GrowthFactor, m_MaxBufferSize, m_RankMPI, m_Buffer, m_Transports, m_BP1Writer, m_Cores );
}

void Writer::Write( Group& group, const std::string variableName, const int* values )
{
    auto index = PreSetVariable( group, variableName, " from call to Write int*" );
    Variable<int>& variable = group.m_Int[index]; //must be a reference
    variable.Values = values;
    WriterWriteVariable( group, variableName, variable, m_GrowthFactor, m_MaxBufferSize, m_RankMPI, m_Buffer, m_Transports, m_BP1Writer, m_Cores );
}

void Writer::Write( Group& group, const std::string variableName, const unsigned int* values )
{
    auto index = PreSetVariable( group, variableName, " from call to Write unsigned int*" );
    Variable<unsigned int>& variable = group.m_UInt[index]; //must be a reference
    variable.Values = values;
    WriterWriteVariable( group, variableName, variable, m_GrowthFactor, m_MaxBufferSize, m_RankMPI, m_Buffer, m_Transports, m_BP1Writer, m_Cores );
}

void Writer::Write( Group& group, const std::string variableName, const long int* values )
{
    auto index = PreSetVariable( group, variableName, " from call to Write long int*" );
    Variable<long int>& variable = group.m_LInt[index]; //must be a reference
    variable.Values = values;
    WriterWriteVariable( group, variableName, variable, m_GrowthFactor, m_MaxBufferSize, m_RankMPI, m_Buffer, m_Transports, m_BP1Writer, m_Cores );
}

void Writer::Write( Group& group, const std::string variableName, const unsigned long int* values )
{
    auto index = PreSetVariable( group, variableName, " from call to Write unsigned long int*" );
    Variable<unsigned long int>& variable = group.m_ULInt[index]; //must be a reference
    variable.Values = values;
    WriterWriteVariable( group, variableName, variable, m_GrowthFactor, m_MaxBufferSize, m_RankMPI, m_Buffer, m_Transports, m_BP1Writer, m_Cores );
}

void Writer::Write( Group& group, const std::string variableName, const long long int* values )
{
    auto index = PreSetVariable( group, variableName, " from call to Write long long int*" );
    Variable<long long int>& variable = group.m_LLInt[index]; //must be a reference
    variable.Values = values;
    WriterWriteVariable( group, variableName, variable, m_GrowthFactor, m_MaxBufferSize, m_RankMPI, m_Buffer, m_Transports, m_BP1Writer, m_Cores );
}

void Writer::Write( Group& group, const std::string variableName, const unsigned long long int* values )
{
    auto index = PreSetVariable( group, variableName, " from call to Write unsigned long long int*" );
    Variable<unsigned long long int>& variable = group.m_ULLInt[index]; //must be a reference
    variable.Values = values;
    WriterWriteVariable( group, variableName, variable, m_GrowthFactor, m_MaxBufferSize, m_RankMPI, m_Buffer, m_Transports, m_BP1Writer, m_Cores );
}

void Writer::Write( Group& group, const std::string variableName, const float* values )
{
    auto index = PreSetVariable( group, variableName, " from call to Write float*" );
    Variable<float>& variable = group.m_Float[index]; //must be a reference
    variable.Values = values;
    WriterWriteVariable( group, variableName, variable, m_GrowthFactor, m_MaxBufferSize, m_RankMPI, m_Buffer, m_Transports, m_BP1Writer, m_Cores );
}


void Writer::Write( Group& group, const std::string variableName, const double* values )
{
    auto index = PreSetVariable( group, variableName, " from call to Write double*" );
    Variable<double>& variable = group.m_Double[index]; //must be a reference
    variable.Values = values;
    WriterWriteVariable( group, variableName, variable, m_GrowthFactor, m_MaxBufferSize, m_RankMPI, m_Buffer, m_Transports, m_BP1Writer, m_Cores );
}


void Writer::Write( Group& group, const std::string variableName, const long double* values )
{
    auto index = PreSetVariable( group, variableName, " from call to Write long double*" );
    Variable<long double>& variable = group.m_LDouble[index]; //must be a reference
    variable.Values = values;
    WriterWriteVariable( group, variableName, variable, m_GrowthFactor, m_MaxBufferSize, m_RankMPI, m_Buffer, m_Transports, m_BP1Writer, m_Cores );
}


void Writer::Write( const std::string variableName, const char* values )
{
    CheckDefaultGroup( );
    Write( *m_Group, variableName, values );
}


void Writer::Write( const std::string variableName, const unsigned char* values )
{
    CheckDefaultGroup( );
    Write( *m_Group, variableName, values );
}


void Writer::Write( const std::string variableName, const short* values )
{
    CheckDefaultGroup( );
    Write( *m_Group, variableName, values );
}


void Writer::Write( const std::string variableName, const unsigned short* values )
{
    CheckDefaultGroup( );
    Write( *m_Group, variableName, values );
}


void Writer::Write( const std::string variableName, const int* values )
{
    CheckDefaultGroup( );
    Write( *m_Group, variableName, values );
}


void Writer::Write( const std::string variableName, const unsigned int* values )
{
    CheckDefaultGroup( );
    Write( *m_Group, variableName, values );
}


void Writer::Write( const std::string variableName, const long int* values )
{
    CheckDefaultGroup( );
    Write( *m_Group, variableName, values );
}


void Writer::Write( const std::string variableName, const unsigned long int* values )
{
    CheckDefaultGroup( );
    Write( *m_Group, variableName, values );
}


void Writer::Write( const std::string variableName, const long long int* values )
{
    CheckDefaultGroup( );
    Write( *m_Group, variableName, values );
}


void Writer::Write( const std::string variableName, const unsigned long long int* values )
{
    CheckDefaultGroup( );
    Write( *m_Group, variableName, values );
}


void Writer::Write( const std::string variableName, const float* values )
{
    CheckDefaultGroup( );
    Write( *m_Group, variableName, values );
}


void Writer::Write( const std::string variableName, const double* values )
{
    CheckDefaultGroup( );
    Write( *m_Group, variableName, values );
}


void Writer::Write( const std::string variableName, const long double* values )
{
    CheckDefaultGroup( );
    Write( *m_Group, variableName, values );
}


void Writer::InitTransports( )
{
    //Let BPFormat handle this??
    std::set< std::string > transportStreamNames; //used to check for name conflict between transports

    const unsigned int transportsSize = m_Method.m_TransportParameters.size();

    for( const auto& parameters : m_Method.m_TransportParameters )
    {
        auto itTransport = parameters.find( "transport" );
        if( m_DebugMode == true )
            CheckParameter( itTransport, parameters, "transport", ", in " + m_Name + m_EndMessage );

        if( itTransport->second == "POSIX" )
        {
            m_Transports.push_back( std::make_shared<POSIX>( m_MPIComm, m_DebugMode ) );
        }
        else if( itTransport->second == "File" )
        {
            m_Transports.push_back( std::make_shared<File>( m_MPIComm, m_DebugMode ) );
        }
        else if( itTransport->second == "FStream" )
        {
            m_Transports.push_back( std::make_shared<FStream>( m_MPIComm, m_DebugMode ) );
        }
        else if( itTransport->second == "MPIFile" )
        {
            //m_Transports.push_back( std::make_shared<MPIFile>( m_MPIComm, m_DebugMode ) );
        }
        else
        {
            if( m_DebugMode == true )
                throw std::invalid_argument( "ERROR: transport + " + itTransport->second + " not supported, in " +
                                              m_Name + m_EndMessage );
        }
        //name
        if( transportsSize > 1 )
        {
            auto itName = parameters.find( "name" ); //first check name

            if( m_DebugMode == true )
                CheckParameter( itName, parameters, "name", " in transport " + itTransport->second +
                                ", in " + m_Name + m_EndMessage );

            m_Transports.back()->Open( itName->second, m_AccessMode );
        }
        else if( transportsSize == 1 )
        {
            auto itName = parameters.find( "name" );

            if( itName == parameters.end() ) //take streamName
                m_Transports.back()->Open( m_Name, m_AccessMode );
            else
                m_Transports.back()->Open( m_Name, m_AccessMode );
        }
        else if( transportsSize == 0 )
        {
            if( m_DebugMode == true )
                throw std::invalid_argument( "ERROR: transport not defined for engine " + m_Name + m_EndMessage );
        }
    }
}


void Writer::Close( const int transportIndex )
{
    //this should be done by BP1Format ?
    if( transportIndex == -1 ) // all transports
    {
        for( auto& transport : m_Transports )
            transport->Write( m_Buffer.m_Data.data(), m_Buffer.m_DataPosition );

        for( auto& transport : m_Transports )
            transport->Close( );
    }
    else
    {
        m_Transports[ transportIndex ]->Write( m_Buffer.m_Data.data(), m_Buffer.m_DataPosition );
        m_Transports[ transportIndex ]->Close( );
    }
}




} //end namespace adios

