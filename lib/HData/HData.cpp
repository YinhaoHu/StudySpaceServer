#include"HData.hpp"

using namespace ceh::Data;
using namespace std;

HData::HData(const char* filename, int _delimiter)
{
	fileObject.open(filename, ios_base::out | ios_base::in);
	fileObject.imbue(std::locale("en_US.UTF-8"));
	delimiter = _delimiter;
}
HData::~HData()
{
	fileObject.close();
}

HDataItem& HData::operator[](size_t idx)
{
	return dataBuffer[idx];
}

void HData::load()
{
	string lineStr;
	HDataItem item;

	dataBuffer.clear();
	while (getline(fileObject, lineStr).good())
	{
		item = HDataItem::fromStdString(lineStr);
		dataBuffer.push_back(item);
	}
	fileObject.clear();
	fileObject.seekp(0);
}
void HData::save()
{
	for (size_t i = 0; i < dataBuffer.size(); ++i)
	{
		fileObject << HDataItem::toStdString(dataBuffer[i]) + "\n";
	}
	fileObject.clear();
	fileObject.seekg(0);
}

bool HData::fail()
{
	return fileObject.fail();
}
size_t HData::size()
{
	return dataBuffer.size();
}

int HData::find(ceh::Data::HDataItem& x)
{
	for (size_t idx = 0; idx < dataBuffer.size(); ++idx)
	{
		if (x == dataBuffer[idx])
			return idx;
	}
	return -1;
}
int HData::find(ceh::Data::HDataItem&& x)
{
	HDataItem lva = x;
	return find(lva);
}
int  HData::findKey(ceh::Data::HDataItem_key& x) {
	for (size_t idx = 0; idx < dataBuffer.size(); ++idx)
	{
		if (x == dataBuffer[idx].key)
			return idx;
	}
	return -1;
}
int  HData::findKey(ceh::Data::HDataItem_key&& x) {
	HDataItem_key lva = x;
	return findKey(lva);
}
int  HData::findValue(ceh::Data::HDataItem_value& x, size_t valueIdx) {
	if (dataBuffer.size() > 0 && dataBuffer[0].values.size() <= valueIdx)
		throw out_of_range("Out of range");
	for (size_t idx = 0; idx < dataBuffer.size(); ++idx)
	{
		if (x == dataBuffer[idx].values[valueIdx])
			return idx;
	}
	return -1;
}
int  HData::findValue(ceh::Data::HDataItem_value&& x, size_t valueIdx) {
	HDataItem_value lva = x;
	return findValue(lva, valueIdx);
}
int  HData::findValues(ceh::Data::HDataItem_values& x) {
	for (size_t idx = 0; idx < dataBuffer.size(); ++idx)
	{
		if (x == dataBuffer[idx].values)
			return idx;
	}
	return -1;
}
int  HData::findValues(ceh::Data::HDataItem_values&& x) {
	HDataItem_values lva = x;
	return findValues(lva);
}


HDataItem& HData::access(size_t idx)
{
	if (idx >= dataBuffer.size())
		throw out_of_range("[Hdata]Access out of range.\n");
	return dataBuffer[idx];
}
bool HData::modify(size_t idx, ceh::Data::HDataItem& newItem)
{
	if (idx >= dataBuffer.size())
		return false;
	else
	{
		dataBuffer[idx] = newItem;
		return true;
	}
}
bool HData::remove(size_t idx)
{
	if (idx >= dataBuffer.size())
		return false;
	else
	{
		dataBuffer.erase(dataBuffer.begin() + idx);
		return true;
	}
}
void HData::append(ceh::Data::HDataItem& newItem)
{
	dataBuffer.push_back(newItem);
}
void HData::append(ceh::Data::HDataItem&& newItem)
{
	dataBuffer.push_back(newItem);
}
