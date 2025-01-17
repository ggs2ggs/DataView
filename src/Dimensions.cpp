// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/Dimensions.hpp>

#include <cmath>
#include <nix/DataArray.hpp>
#include <nix/util/util.hpp>
#include <nix/Exception.hpp>

using namespace std;
using namespace nix;
using namespace nix::base;


//-------------------------------------------------------
// Implementation of Dimension
//-------------------------------------------------------

Dimension::Dimension()
    : ImplContainer()
{
}


Dimension::Dimension(const shared_ptr<IDimension> &p_impl)
    : ImplContainer(p_impl)
{
}


Dimension::Dimension(shared_ptr<IDimension> &&ptr)
    : ImplContainer(std::move(ptr))
{
}


Dimension::Dimension(const Dimension &other)
    : ImplContainer(other)
{
}


Dimension::Dimension(const SampledDimension &other)
    : ImplContainer(dynamic_pointer_cast<IDimension>(other.impl()))
{
}


Dimension::Dimension(const RangeDimension &other)
    : ImplContainer(dynamic_pointer_cast<IDimension>(other.impl()))
{
}


Dimension::Dimension(const SetDimension &other)
    : ImplContainer(dynamic_pointer_cast<IDimension>(other.impl()))
{
}


SetDimension Dimension::asSetDimension() const {
    if (dimensionType() != DimensionType::Set) {
        throw IncompatibleDimensions("Dimension is not of type Set and thus cannot be cast to this type", "asSetDimension");
    }
    return SetDimension(std::dynamic_pointer_cast<base::ISetDimension>(impl()));
}


SampledDimension Dimension::asSampledDimension() const {
    if (dimensionType() != DimensionType::Sample) {
        throw IncompatibleDimensions("Dimension is not of type Sample and thus cannot be cast to this type", "asSampledDimension");
    }
    return SampledDimension(std::dynamic_pointer_cast<base::ISampledDimension>(impl()));
}


RangeDimension Dimension::asRangeDimension() const {
    if (dimensionType() != DimensionType::Range) {
        throw IncompatibleDimensions("Dimension is not of type Range and thus cannot be cast to this type", "asRangeDimension");
    }
    return RangeDimension(std::dynamic_pointer_cast<base::IRangeDimension>(impl()));
}


Dimension& Dimension::operator=(const SampledDimension &other) {
    shared_ptr<IDimension> tmp(dynamic_pointer_cast<IDimension>(other.impl()));

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}


Dimension& Dimension::operator=(const RangeDimension &other) {
    shared_ptr<IDimension> tmp(dynamic_pointer_cast<IDimension>(other.impl()));

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}


Dimension& Dimension::operator=(const SetDimension &other) {
    shared_ptr<IDimension> tmp(dynamic_pointer_cast<IDimension>(other.impl()));

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}

//-------------------------------------------------------
// Implementation of SampledDimension
//-------------------------------------------------------

SampledDimension::SampledDimension()
    : ImplContainer()
{
}


SampledDimension::SampledDimension(const std::shared_ptr<ISampledDimension> &p_impl)
    : ImplContainer(p_impl)
{
}


SampledDimension::SampledDimension(std::shared_ptr<ISampledDimension> &&ptr)
    : ImplContainer(std::move(ptr))
{
}



SampledDimension::SampledDimension(const SampledDimension &other)
    : ImplContainer(other)
{
}


void SampledDimension::label(const std::string &label) {
    util::checkEmptyString(label, "label");
    backend()->label(label);
}


void SampledDimension::unit(const std::string &unit) {
    util::checkEmptyString(unit, "unit");
    if (!(util::isSIUnit(unit))) {
        throw InvalidUnit("Unit is not a SI unit. Note: so far, only atomic SI units are supported.",
                          "SampledDimension::unit(const string &unit)");
    }
    backend()->unit(unit);
}


void SampledDimension::samplingInterval(double interval) {
    if (interval <= 0.0) {
        throw std::runtime_error("SampledDimenion::samplingInterval: Sampling intervals must be larger than 0.0!");
    }
    backend()->samplingInterval(interval);
}


size_t SampledDimension::indexOf(const double position) const {
    //FIXME: should we use NDSSize::value_type here instead of ssize_t?
    //FIXME: also, on the smae grounds, should the return not be NDSize::value_type?
    ssize_t index;
    double offset = backend()->offset() ? *(backend()->offset()) : 0.0;
    double sampling_interval = backend()->samplingInterval();
    index = static_cast<ssize_t>(round(( position - offset) / sampling_interval));
    if (index < 0) {
        throw nix::OutOfBounds("Position is out of bounds of this dimension!", 0);
    }
    return static_cast<size_t>(index);
}


double SampledDimension::positionAt(const size_t index) const {
    double offset = backend()->offset() ? *(backend()->offset()) : 0.0;
    double sampling_interval = backend()->samplingInterval();
    return index * sampling_interval + offset;
}


vector<double> SampledDimension::axis(const size_t count, const size_t startIndex) const {
    vector<double> axis(count);
    double offset =  backend()->offset() ? *(backend()->offset()) : 0.0;
    double sampling_interval = backend()->samplingInterval();
    for (size_t i = 0; i < axis.size(); ++i) {
        axis[i] = (i + startIndex) * sampling_interval + offset;
    }
    return axis;
}


SampledDimension& SampledDimension::operator=(const SampledDimension &other) {
    shared_ptr<ISampledDimension> tmp(other.impl());

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}


SampledDimension& SampledDimension::operator=(const Dimension &other) {
    shared_ptr<ISampledDimension> tmp(dynamic_pointer_cast<ISampledDimension>(other.impl()));

    if (other.dimensionType() != DimensionType::Sample) {
        throw nix::IncompatibleDimensions("Cannot assign dimension of type " + nix::util::dimTypeToStr(other.dimensionType())
                                          + " to a SampledDimension", "SampledDimension::operator=");
    }
    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }
    return *this;
}

//-------------------------------------------------------
// Implementation of SetDimension
//-------------------------------------------------------


SetDimension::SetDimension()
    : ImplContainer()
{
}


SetDimension::SetDimension(const std::shared_ptr<ISetDimension> &p_impl)
    : ImplContainer(p_impl)
{
}

SetDimension::SetDimension(std::shared_ptr<ISetDimension> &&ptr)
    : ImplContainer(std::move(ptr))
{
}


SetDimension::SetDimension(const SetDimension &other)
    : ImplContainer(other)
{
}


SetDimension& SetDimension::operator=(const SetDimension &other) {
    shared_ptr<ISetDimension> tmp(other.impl());

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}


SetDimension& SetDimension::operator=(const Dimension &other) {
    shared_ptr<ISetDimension> tmp(dynamic_pointer_cast<ISetDimension>(other.impl()));

    if (other.dimensionType() != DimensionType::Set) {
        throw nix::IncompatibleDimensions("Cannot assign dimension of type " + nix::util::dimTypeToStr(other.dimensionType())
                                          + " to a SetDimension", "SetDimension::operator=");
    }
    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}

//-------------------------------------------------------
// Implementation of RangeDimension
//-------------------------------------------------------


RangeDimension::RangeDimension()
    : ImplContainer()
{
}


RangeDimension::RangeDimension(const DataArray &array)
    : ImplContainer()
{
    if (array.dataExtent().size() > 1) {
        throw InvalidRank("Error creating RangeDimension with DataArray: array must be 1-D!");
    }
}


RangeDimension::RangeDimension(const std::shared_ptr<IRangeDimension> &p_impl)
    : ImplContainer(p_impl)
{
}


RangeDimension::RangeDimension(std::shared_ptr<IRangeDimension> &&ptr)
    : ImplContainer(std::move(ptr))
{
}


RangeDimension::RangeDimension(const RangeDimension &other)
    : ImplContainer(other)
{
}


void RangeDimension::label(const std::string &label) {
    util::checkEmptyString(label, "label");
    backend()->label(label);
}

void RangeDimension::unit(const std::string &unit) {
    util::checkEmptyString(unit, "unit");
    if (!(util::isSIUnit(unit))) {
        throw InvalidUnit("Unit is not an atomic SI. Note: So far composite units are not supported", "RangeDimension::unit(const string &unit)");
    }
    backend()->unit(unit);
}


void RangeDimension::ticks(const std::vector<double> &ticks) {
    if (!std::is_sorted(ticks.begin(), ticks.end())) {
        std::string caller = "Range::ticks()";
        throw UnsortedTicks(caller);
    }
    backend()->ticks(ticks);
}


double RangeDimension::tickAt(const size_t index) const {
    vector<double> ticks = this->ticks();
    if (index >= ticks.size()) {
        throw nix::OutOfBounds("RangeDimension::tickAt: Given index is out of bounds!", index);
    }
    return ticks[index];
}


size_t RangeDimension::indexOf(const double position) const {
    vector<double> ticks = this->ticks();
    if (position < *ticks.begin()) {
        return 0;
    } else if (position > *prev(ticks.end())) {
        return prev(ticks.end()) - ticks.begin();
    }
    vector<double>::iterator low = std::lower_bound (ticks.begin(), ticks.end(), position);
    return low - ticks.begin();
}


vector<double> RangeDimension::axis(const size_t count, const size_t startIndex) const {
    vector<double> ticks = this->ticks();
    if ((startIndex + count) > ticks.size()) {
        throw nix::OutOfBounds("RangeDimension::axis: Count is invalid, reaches beyond the ticks stored in this dimension.");
    } 
    vector<double>::const_iterator first = ticks.begin() + startIndex;
    vector<double> axis(first, first + count);
    return axis;
}


RangeDimension& RangeDimension::operator=(const RangeDimension &other) {
    shared_ptr<IRangeDimension> tmp(other.impl());

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}


RangeDimension& RangeDimension::operator=(const Dimension &other) {
    shared_ptr<IRangeDimension> tmp(dynamic_pointer_cast<IRangeDimension>(other.impl()));

    if (other.dimensionType() != DimensionType::Range) {
        throw nix::IncompatibleDimensions("Cannot assign dimension of type " + nix::util::dimTypeToStr(other.dimensionType())
                                          + " to a RangeDimension", "RangeDimension::operator=");
    }
    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}




