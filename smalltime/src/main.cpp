#include <iostream>		
#include <vector>
#include <array>

#include "../include/local_datetime.h"
#include <basic_datetime.h>
#include <core_math.h>
#include <time_math.h>
#include <util/stl_perf_counter.h>

#include <rule_group.h>
#include <zone_group.h>
#include "../include/tzdb_header_connector.h"
#include <tzdb_connector_interface.h>
#include <memory>
#include <float_util.h>
#include <inttypes.h>

using namespace smalltime;

int main()
{
	StlPerfCounter counter("Counter");
	counter.StartCounter();

	std::shared_ptr<tz::TzdbHeaderConnector> tzdb_connector = std::make_shared<tz::TzdbHeaderConnector>();

	auto rule_handle = tzdb_connector->GetRuleHandle();
	auto rule_id = math::GetUniqueID("US");
	auto rules = tzdb_connector->FindRules("US");

	auto zoneHandle = tzdb_connector->GetZoneHandle();
	//auto zones = tzdb_connector->FindZones("America/Anchorage");
	auto zones = tzdb_connector->FindZones("America/New_York");


	std::cout << "Rules ID: " << rules.rule_id << " Rules first: " << rules.first << " Rules size: " << rules.size << std::endl;
	std::cout << "Zones ID: " << zones.zone_id << " Zones first: " << zones.first << " Zones size: " << zones.size << std::endl;

	tz::RuleGroup ru(rule_id, &zoneHandle[zones.first + zones.size - 1], std::dynamic_pointer_cast<tz::TzdbConnectorInterface, tz::TzdbHeaderConnector>(tzdb_connector));
	tz::ZoneGroup zu(zones, std::dynamic_pointer_cast<tz::TzdbConnectorInterface, tz::TzdbHeaderConnector>(tzdb_connector));


	BasicDateTime<> dt1(1966, 12, 31, 23, 59, 59, 999, tz::TimeType::KTimeType_Wall);
	BasicDateTime<> dt0(1983, 10, 30, 1, 0, 0, 0, tz::TimeType::KTimeType_Wall);

	BasicDateTime<> dt(2016, 11, 6, 1, 0, 0, 0, tz::TimeType::KTimeType_Utc);

	/*
	for (int i = zones.first; i < zones.first + zones.size; ++i)
	{
		auto z = zoneHandle[i];
		BasicDateTime<> cr(z.until_wall, z.until_type);
		printf("CR ############## - %d/%d/%d %d:%d:%d:%d\n", cr.GetYear(), cr.GetMonth(), cr.GetDay(), cr.GetHour(), cr.GetMinute(), cr.GetSecond(), cr.GetMillisecond());
	}

	for (int i = rules.first; i < rules.first + rules.size; ++i)
	{
		auto z = rule_handle[i];
		BasicDateTime<> cr = ru.CalcTransitionWall(&z, 1983);
		printf("RT ############## - %d/%d/%d %d:%d:%d:%d\n", cr.GetYear(), cr.GetMonth(), cr.GetDay(), cr.GetHour(), cr.GetMinute(), cr.GetSecond(), cr.GetMillisecond());
	}
	*/
	
	try
	{
		auto ar = ru.FindActiveRule(dt, Choose::KError);
	//	auto z = zu.FindActiveZone(dt0, Choose::KError);
		printf("AR From - %d  To - %d  In - %d  On - %d\n", ar->from_year, ar->to_year, ar->month, ar->day);


	//	BasicDateTime<> cr(z->until_wall, z->until_type);
	//	printf("CR ############## - %d/%d/%d %d:%d:%d:%d\n", cr.GetYear(), cr.GetMonth(), cr.GetDay(), cr.GetHour(), cr.GetMinute(), cr.GetSecond(), cr.GetMillisecond());

	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}


	

	counter.EndCounter();

	std::cout << "ms elapsed =  " << counter.GetElapsedMilliseconds() << " us elapsed = " << counter.GetElapsedMicroseconds() << std::endl;

	std::cin.get();


	return 0;
}