#include "UnitTest++/UnitTest++.h"

#include "pugixml.hpp"

#include "xs_mesh_homogenized.hpp"

using namespace mocc;
using std::cout;
using std::endl;

TEST( xsmeshhom ) {
    {
        pugi::xml_document geom_xml;
        pugi::xml_parse_result result = geom_xml.load_file( "2x3_1.xml" );
        CHECK( result );

        CoreMesh mesh( geom_xml );

        XSMeshHomogenized xs_mesh( mesh );

        H5Node h5f( "xsmesh_1.h5", H5Access::WRITE );
        auto g = h5f.create_group("xs_mesh");
        xs_mesh.output(g);
    }
    {
        pugi::xml_document geom_xml;
        pugi::xml_parse_result result = geom_xml.load_file( "2x3_2.xml" );
        CHECK( result );

        CoreMesh mesh( geom_xml );

        XSMeshHomogenized xs_mesh( mesh );

        H5Node h5f( "xsmesh_2.h5", H5Access::WRITE );
        xs_mesh.output(h5f);
    }
    
}

// Tests some of the error checking involved in constructing an XSMeshHom from
// data files.
TEST( fromdata_fail ) {
    pugi::xml_document geom_xml;
    pugi::xml_parse_result result = geom_xml.load_file( "2x3_stack.xml" );
    CHECK( result );
    if( !result ) {
        std::cout << result.description() << std::endl;
    }

    CoreMesh mesh( geom_xml );

    // Test error checks on the XML input
    {
        cout << "invalid" << endl;
        pugi::xml_document xsmesh_xml;
        std::string xml = 
            "<data file=\"xsmesh_1.h5\" top_plane=\"-1\"/>";
        xsmesh_xml.load_string( xml.c_str() );

        CHECK_THROW( XSMeshHomogenized xs_mesh( mesh, xsmesh_xml ), Exception);
    }
    {
        cout << "out of order" << endl;
        pugi::xml_document xsmesh_xml;
        std::string xml = 
            "<data file=\"xsmesh_1.h5\" top_plane=\"5\"/>"
            "<data file=\"xsmesh_2.h5\" top_plane=\"1\"/>";
        xsmesh_xml.load_string( xml.c_str() );

        CHECK_THROW( XSMeshHomogenized xs_mesh( mesh, xsmesh_xml ), Exception);
    }

}

// Test an actual xsmesh hom object that should successfully construct
TEST( fromdata )
{
    pugi::xml_document geom_xml;
    pugi::xml_parse_result result = geom_xml.load_file( "2x3_stack.xml" );
    CHECK( result );
    if( !result ) {
        std::cout << result.description() << std::endl;
    }

    CoreMesh mesh( geom_xml );

    pugi::xml_document xsmesh_xml;
    std::string xml = 
        "<data file=\"xsmesh_2.h5\" top_plane=\"7\"/>"
        "<data file=\"xsmesh_1.h5\" top_plane=\"11\"/>"
        "";
    xsmesh_xml.load_string( xml.c_str() );

    XSMeshHomogenized xs_mesh( mesh, xsmesh_xml );

    CHECK_EQUAL( 7, xs_mesh.eubounds().size() );

    CHECK_EQUAL( 72, xs_mesh.size() );

    CHECK_CLOSE( 2.005998E-02, xs_mesh[48].xsmacnf(0),
                 0.000001 );
    CHECK_CLOSE( 2.005998E-02, xs_mesh[54].xsmacnf(0),
                 0.000001 );
    CHECK_CLOSE( 2.005998E-02, xs_mesh[60].xsmacnf(0),
                 0.000001 );
    CHECK_CLOSE( 2.005998E-02, xs_mesh[66].xsmacnf(0),
                 0.000001 );

    CHECK_CLOSE( 0.0125521, xs_mesh[0].xsmacnf(0),
                 0.000001 );
    CHECK_CLOSE( 0.0125521, xs_mesh[24].xsmacnf(0),
                 0.000001 );
    CHECK_CLOSE( 0.0125521, xs_mesh[30].xsmacnf(0),
                 0.000001 );
    CHECK_CLOSE( 0.0125521, xs_mesh[47].xsmacnf(0),
                 0.000001 );

    // test this whole plane. It's got some asymmetry, so if everything is good
    // here, we can be pretty certain of the X- and Y-dimensions in the transfer
    CHECK_CLOSE( 0.0125521, xs_mesh[36].xsmacnf(0),
                 0.000001 );
    CHECK_CLOSE( 0.0125521, xs_mesh[37].xsmacnf(0),
                 0.000001 );
    CHECK_CLOSE( 0.0115752, xs_mesh[38].xsmacnf(0),
                 0.000001 );
    CHECK_CLOSE( 0.0115752, xs_mesh[39].xsmacnf(0),
                 0.000001 );
    CHECK_CLOSE( 0.0115752, xs_mesh[40].xsmacnf(0),
                 0.000001 );
    CHECK_CLOSE( 0.0125521, xs_mesh[41].xsmacnf(0),
                 0.000001 );


    // Now for the big guns: Re-make the cross section mesh directly from the
    // core mesh via homogenization and check all of the fields. This doesnt
    // test the actual homogenization procedures, but its an excellent test of
    // the I/O procedures.
    XSMeshHomogenized xs_reference( mesh );

    CHECK( xs_mesh == xs_reference );

}

int main() {
    return UnitTest::RunAllTests();
}
