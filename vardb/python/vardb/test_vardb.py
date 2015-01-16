

from vardb import VDBFile, VDBVar

import os.path

_testdir = os.path.join(os.path.dirname(__file__), "../../tests")
_deepwave_xml = os.path.join(_testdir, "deepwave_vardb.xml")

def test_open_and_save():
    vdb = VDBFile("")
    dir(vdb)
    vdb.open(_deepwave_xml)
    assert(vdb.is_valid())
    qcf = vdb.get_var("QCF")
    assert(qcf.name() == "QCF")
    assert(qcf.get_attribute("units") == "hPa")
    assert(qcf.get_attribute(VDBVar.LONG_NAME) == 
           "Raw Dynamic Pressure, Fuselage")
    assert(qcf.get_attribute(VDBVar.UNITS) == "hPa")
    assert(qcf.get_attribute(VDBVar.STANDARD_NAME) == "")
    assert(qcf.get_attribute(VDBVar.REFERENCE) == "false")
    assert(qcf.get_attribute(VDBVar.IS_ANALOG) == "true")
    assert(qcf.get_attribute(VDBVar.VOLTAGE_RANGE) == "0 10")
    assert(qcf.get_attribute(VDBVar.DEFAULT_SAMPLE_RATE) == "100")
    assert(qcf.get_attribute(VDBVar.CATEGORY) == "Uncorr'd Raw")
    # Missing attribute returns empty string.
    assert(qcf.get_attribute(VDBVar.MODULUS_RANGE) == "")

    # Missing attribute returns a supplied default.
    assert(qcf.get_attribute(VDBVar.STANDARD_NAME, "air_pressure") ==
           "air_pressure")

    # vdb.save("saved.xml")
    vdb.close()


def test_categories():
    vdb = VDBFile(_deepwave_xml)
    categories = vdb.get_categories()
    assert(categories == ["Position", "Thermodynamic", "Aircraft State",
                          "Atmos. State", "Liquid Water", 
                          "Uncorr'd Raw", "Wind", "PMS Probe",
                          "Housekeeping", "Chemistry", "Radiation",
                          "Non-Standard"])
    vdb.close()


def test_standard_names():
    vdb = VDBFile(_deepwave_xml)
    standard_names = vdb.get_standard_names()
    assert(standard_names ==
           ["altitude", "air_potential_temperature", "air_pressure",
            "air_temperature", "dew_point_temperature", "geopotential_height",
            "eastward_wind", "latitude", "longitude", "northward_wind",
            "relative_humidity", "surface_air_pressure",
            "platform_speed_wrt_air", "upward_air_velocity",
            "wind_from_direction", "wind_speed", "water_vapor_pressure",
            "solar_zenith_angle", "platform_course", "platform_orientation",
            "platform_pitch_angle", "platform_roll_angle",
            "platform_speed_wrt_ground", "humidity_mixing_ratio",
            "equivelent_potential_temperature", "height", 
            "atmosphere_cloud_liquid_water_content",
            "air_pressure_at_sea_level", "solar_azimuth_angle",
            "solar_elevation_angle", "virtual_temperature",
            "atmosphere_number_content_of_aerosol_particles",
            "mole_fraction_of_carbon_dioxide_in_air",
            "mole_fraction_of_methane_in_air",
            "mole_fraction_of_carbon_monoxide_in_air",
            "mole_fraction_of_ozone_in_air"])
    vdb.close()

