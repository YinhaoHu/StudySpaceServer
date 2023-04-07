#include"HData.hpp"

#include<locale>

using namespace ceh::Data;
using namespace std;

HWData::HWData(const char* filename, wchar_t _delimiter)
{
	fileObject.open(filename, ios_base::out | ios_base::in);
	fileObject.imbue(std::locale("en_US.UTF-8"));
	delimiter = _delimiter;
}
HWData::~HWData()
{
	fileObject.close();
}

HWDataItem& HWData::operator[](size_t idx)
{
	return dataBuffer[idx];
}

void HWData::load()
{
	wstring lineStr;
	HWDataItem item;

	dataBuffer.clear();
	while (getline(fileObject, lineStr,L'\n').good())
	{
		item = HWDataItem::fromStdWString(lineStr);
		dataBuffer.push_back(item);
	}
	fileObject.clear();
	fileObject.seekp(0);
}
void HWData::save()
{
	for (size_t i = 0; i < dataBuffer.size(); ++i)
	{
		fileObject << HWDataItem::toStdWString(dataBuffer[i]) + L"\n";
	}
	fileObject.clear();
	fileObject.seekg(0);
}

bool HWData::fail()
{
	return fileObject.fail();
}
size_t HWData::size()
{
	return dataBuffer.size();
}

int HWData::find(ceh::Data::HWDataItem& x)
{
	for (size_t idx = 0; idx < dataBuffer.size(); ++idx)
	{
		if (x == dataBuffer[idx])
			return idx;
	}
	return -1;
}
int HWData::find(ceh::Data::HWDataItem&& x)
{
	HWDataItem lva = x;
	return find(lva);
}
int HWData::findKey(ceh::Data::HWDataItem_key& x) {
	for (size_t idx = 0; idx < dataBuffer.size(); ++idx)
	{
		if (x == dataBuffer[idx].key)
			return idx;
	}
	return -1;
}
int HWData::findKey(ceh::Data::HWDataItem_key&& x) {
	HWDataItem_key lva = x;
	return findKey(lva);
}
int HWData::findValue(ceh::Data::HWDataItem_value& x, size_t valueIdx) {
	if (dataBuffer.size() > 0 && dataBuffer[0].values.size() <= valueIdx)
		throw out_of_range("Out of range");
	for (size_t idx = 0; idx < dataBuffer.size(); ++idx)
	{
		if (x == dataBuffer[idx].values[valueIdx])
			return idx;
	}
	return -1;
}
int HWData::findValue(ceh::Data::HWDataItem_value&& x, size_t valueIdx) {
	HWDataItem_value lva = x;
	return findValue(lva, valueIdx);
}
int HWData::findValues(ceh::Data::HWDataItem_values& x) {
	for (size_t idx = 0; idx < dataBuffer.size(); ++idx)
	{
		if (x == dataBuffer[idx].values)
			return idx;
	}
	return -1;
}
int HWData::findValues(ceh::Data::HWDataItem_values&& x) {
	HWDataItem_values lva = x;
	return findValues(lva);
}
HWDataItem& HWData::access(size_t idx)
{
	if (idx >= dataBuffer.size())
		throw out_of_range("[HWData]Access out of range.\n");
	return dataBuffer[idx];
}
bool HWData::modify(size_t idx, ceh::Data::HWDataItem& newItem)
{
	if (idx >= dataBuffer.size())
		return false;
	else
	{
		dataBuffer[idx] = newItem;
		return true;
	}
}
bool HWData::remove(size_t idx)
{
	if (idx >= dataBuffer.size())
		return false;
	else
	{
		dataBuffer.erase(dataBuffer.begin() + idx);
		return true;
	}
}
void HWData::append(ceh::Data::HWDataItem& newItem)
{
	dataBuffer.push_back(newItem);
}
void HWData::append(ceh::Data::HWDataItem&& newItem)
{
	dataBuffer.push_back(newItem);
}
