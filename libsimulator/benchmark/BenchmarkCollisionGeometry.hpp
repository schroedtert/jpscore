/// Copyright © 2012-2022 Forschungszentrum Jülich GmbH
/// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include <benchmark/benchmark.h>

#include "CollisionGeometry.hpp"
#include "LineSegment.hpp"
#include "GeometryBuilder.hpp"
#include "BuildGeometries.hpp"

class LargeStreetNetworkFixture : public benchmark::Fixture{
public:
    Geometry geometry;

    void SetUp(const ::benchmark::State& state){
        geometry = buildLargeStreetNetwork();
    }
};

class GrosserSternFixture : public benchmark::Fixture{
public:
    Geometry geometry;

    void SetUp(const ::benchmark::State& state){
        geometry = buildGrosserStern();
    }
};

BENCHMARK_F(LargeStreetNetworkFixture, bmIntersectsAny)(benchmark::State& state){//    LineSegment lineSegment(Point(-1., 1.), Point(1., -1.));
    LineSegment lineSegment(Point(-1., 1.), Point(100., -100.));

    for (auto _ : state){
        geometry.collisionGeometry->IntersectsAny(lineSegment);
    }
}

BENCHMARK_F(GrosserSternFixture, bmIntersectsAny)(benchmark::State& state){//    LineSegment lineSegment(Point(-1., 1.), Point(1., -1.));
    LineSegment lineSegment(Point(-1., 1.), Point(100., -100.));

    for (auto _ : state){
        geometry.collisionGeometry->IntersectsAny(lineSegment);
    }
}

BENCHMARK_F(LargeStreetNetworkFixture, bmIntersectsAnySet)(benchmark::State& state){//    LineSegment lineSegment(Point(-1., 1.), Point(1., -1.));
    LineSegment lineSegment(Point(-1., 1.), Point(100., -100.));

    for (auto _ : state){
        geometry.collisionGeometry->IntersectsAnySet(lineSegment);
    }
}
BENCHMARK_F(GrosserSternFixture, bmIntersectsAnySet)(benchmark::State& state){//    LineSegment lineSegment(Point(-1., 1.), Point(1., -1.));
    LineSegment lineSegment(Point(-1., 1.), Point(100., -100.));

    for (auto _ : state){
        geometry.collisionGeometry->IntersectsAnySet(lineSegment);
    }
}

//template<class ...Args>
//void bmIntersectsAny(benchmark::State& state, Args&&... args)
//{
////    auto geometry = buildGrosserStern();
//    auto args_tuple = std::make_tuple(std::move(args)...);
//    auto geometry = std::get<0>(args_tuple);
//    LineSegment lineSegment(Point(-1., 1.), Point(1., -1.));
//    for (auto _ : state){
//        geometry.collisionGeometry->IntersectsAny(lineSegment);
////        std::cout << "bmIntersectsAny!" << std::endl;
//    }
//}
//// Register the function as a benchmark
//BENCHMARK_CAPTURE(bmIntersectsAny, buildGrosserStern());
//BENCHMARK_CAPTURE(bmIntersectsAny, buildLargeStreetNetwork());
