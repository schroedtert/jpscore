/// Copyright © 2012-2022 Forschungszentrum Jülich GmbH
/// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include <benchmark/benchmark.h>

#include "BuildGeometries.hpp"
#include "CollisionGeometry.hpp"
#include "GeometryBuilder.hpp"
#include "LineSegment.hpp"

template <class... Args>
void bmIntersectsAny(benchmark::State& state, Args&&... args)
{
    auto args_tuple = std::make_tuple(std::move(args)...);
    auto geometry = std::move(std::get<Geometry>(args_tuple));
    auto lineSegment = std::move(std::get<LineSegment>(args_tuple));

    for(auto _ : state) {
        geometry.collisionGeometry->IntersectsAny(lineSegment);
    }
}

template <class... Args>
void bmIntersectsAnyExtend(benchmark::State& state, Args&&... args)
{
    auto args_tuple = std::make_tuple(std::move(args)...);
    auto geometry = std::move(std::get<Geometry>(args_tuple));
    auto factor = state.range(0);
    auto start_point = std::move(std::get<Point>(args_tuple));

    auto lineSegment = LineSegment(start_point, Point(start_point.x + factor * CELL_EXTEND, start_point.y));
    for(auto _ : state) {
        geometry.collisionGeometry->IntersectsAny(lineSegment);
    }
}

template <class... Args>
void bmIntersectsAnySet(benchmark::State& state, Args&&... args)
{
    auto args_tuple = std::make_tuple(std::move(args)...);
    auto geometry = std::move(std::get<Geometry>(args_tuple));
    auto lineSegment = std::move(std::get<LineSegment>(args_tuple));

    for(auto _ : state) {
        geometry.collisionGeometry->IntersectsAnySet(lineSegment);
    }
}

template <class... Args>
void bmIntersectsAnySetExtend(benchmark::State& state, Args&&... args)
{
    auto args_tuple = std::make_tuple(std::move(args)...);
    auto geometry = std::move(std::get<Geometry>(args_tuple));
    auto factor = state.range(0);
    auto start_point = std::move(std::get<Point>(args_tuple));

    auto lineSegment = LineSegment(start_point, Point(start_point.x + factor * CELL_EXTEND, start_point.y + factor * CELL_EXTEND));
    for(auto _ : state) {
        geometry.collisionGeometry->IntersectsAnySet(lineSegment);
    }
}

template <class... Args>
void bmIntersectsAnyVector(benchmark::State& state, Args&&... args)
{
    auto args_tuple = std::make_tuple(std::move(args)...);
    auto geometry = std::move(std::get<Geometry>(args_tuple));
    auto lineSegment = std::move(std::get<LineSegment>(args_tuple));

    for(auto _ : state) {
        geometry.collisionGeometry->IntersectsAnyVector(lineSegment);
    }
}

template <class... Args>
void bmIntersectsAnyVectorExtend(benchmark::State& state, Args&&... args)
{
    auto args_tuple = std::make_tuple(std::move(args)...);
    auto geometry = std::move(std::get<Geometry>(args_tuple));
    auto factor = state.range(0);
    auto start_point = std::move(std::get<Point>(args_tuple));

    auto lineSegment = LineSegment(start_point, Point(start_point.x + factor * CELL_EXTEND, start_point.y + factor * CELL_EXTEND));
    for(auto _ : state) {
        geometry.collisionGeometry->IntersectsAnyVector(lineSegment);
    }
}

// Test case 1: Line segments in same cell
// Test case 2: Line segments in neighboring cell
// Test case 3: Line segments in non-neighboring cell
// Test case 4: Line segments in opposite ends of geometry

//
// Grosser_stern
//
BENCHMARK_CAPTURE(
    bmIntersectsAnyExtend,
    grosser_stern_extend_cell,
    buildGrosserStern(),
    Point(-1., -1.))
    ->Arg(0)
    ->Arg(1)
    ->DenseRange(2, 50, 2)
    ->DenseRange(75, 150, 25);
BENCHMARK_CAPTURE(
    bmIntersectsAny,
    grosser_stern_opposite_ends,
    buildGrosserStern(),
    LineSegment(Point(-2320.188, -606.225), Point(-1643.801, 65.862)));

//BENCHMARK_CAPTURE(
//    bmIntersectsAnySetExtend,
//    grosser_stern_extend_cell,
//    buildGrosserStern(),
//    Point(-2320.188, -606.225))
//    ->Arg(0)
//    ->Arg(1)
//    ->DenseRange(2, 50, 2)
//    ->DenseRange(75, 150, 25);
//BENCHMARK_CAPTURE(
//    bmIntersectsAnySet,
//    grosser_stern_opposite_ends,
//    buildGrosserStern(),
//    LineSegment(Point(-2320.188, -606.225), Point(-1643.801, 65.862)));
//
//BENCHMARK_CAPTURE(
//    bmIntersectsAnyVectorExtend,
//    grosser_stern_extend_cell,
//    buildGrosserStern(),
//    Point(-2320.188, -606.225))
//    ->Arg(0)
//    ->Arg(1)
//    ->DenseRange(2, 50, 2)
//    ->DenseRange(75, 150, 25);
//BENCHMARK_CAPTURE(
//    bmIntersectsAnyVector,
//    grosser_stern_opposite_ends,
//    buildGrosserStern(),
//    LineSegment(Point(-2320.188, -606.225), Point(-1643.801, 65.862)));
//
////
//// Largestreet network
////
//BENCHMARK_CAPTURE(
//    bmIntersectsAnyExtend,
//    large_street_network_extend_cell,
//    buildLargeStreetNetwork(),
//    Point(-5.26, -7.753))
//    ->Arg(0)
//    ->Arg(1)
//    ->DenseRange(2, 50, 2)
//    ->DenseRange(75, 450, 25);
//BENCHMARK_CAPTURE(
//    bmIntersectsAny,
//    large_street_network_opposite_ends,
//    buildLargeStreetNetwork(),
//    LineSegment(Point(-5.26, -7.753), Point(-1805.37, 2434.899)));
//
//BENCHMARK_CAPTURE(
//    bmIntersectsAnySetExtend,
//    large_street_network_extend_cell,
//    buildLargeStreetNetwork(),
//    Point(-5.26, -7.753))
//    ->Arg(0)
//    ->Arg(1)
//    ->DenseRange(2, 50, 2)
//    ->DenseRange(75, 450, 25);
//BENCHMARK_CAPTURE(
//    bmIntersectsAnySet,
//    large_street_network_opposite_ends,
//    buildLargeStreetNetwork(),
//    LineSegment(Point(-5.26, -7.753), Point(-1805.37, 2434.899)));
//
//BENCHMARK_CAPTURE(
//    bmIntersectsAnyVectorExtend,
//    large_street_network_extend_cell,
//    buildLargeStreetNetwork(),
//    Point(-5.26, -7.753))
//    ->Arg(0)
//    ->Arg(1)
//    ->DenseRange(2, 50, 2)
//    ->DenseRange(75, 450, 25);
//
//BENCHMARK_CAPTURE(
//    bmIntersectsAnyVector,
//    large_street_network_opposite_ends,
//    buildLargeStreetNetwork(),
//    LineSegment(Point(-5.26, -7.753), Point(-1805.37, 2434.899)));
