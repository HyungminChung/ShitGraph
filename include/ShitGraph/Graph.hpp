#pragma once

#include <ShitGraph/CoreType.hpp>
#include <ShitGraph/Graphic.hpp>
#include <ShitGraph/Sampler.hpp>

#include <type_traits>
#include <utility>
#include <vector>

namespace ShitGraph {
	enum class IndependentVariable {
		X,
		Y,
	};
}

namespace ShitGraph {
	struct GraphClass {
		ShitGraph::IndependentVariable IndependentVariable = IndependentVariable::X;

		ShitGraph::Color Color;
		Scalar Width = 2;
		bool Visible = true;
	};

	template<typename T>
	std::enable_if_t<std::is_base_of_v<GraphClass, std::decay_t<T>>, T&&> MakeForY(T&& graphClass) noexcept {
		return graphClass.IndependentVariable = IndependentVariable::Y, std::forward<T>(graphClass);
	}
	template<typename T>
	std::enable_if_t<std::is_base_of_v<GraphClass, std::decay_t<T>>, T&&> MakeForX(T&& graphClass) noexcept {
		return graphClass.IndependentVariable = IndependentVariable::X, std::forward<T>(graphClass);
	}
	template<typename T>
	std::enable_if_t<std::is_base_of_v<GraphClass, std::decay_t<T>>, T&&> ChangeColor(T&& graphClass, Color color) noexcept {
		return graphClass.Color = color, std::forward<T>(graphClass);
	}
	template<typename T>
	std::enable_if_t<std::is_base_of_v<GraphClass, std::decay_t<T>>, T&&> ChangeWidth(T&& graphClass, Scalar width) noexcept {
		return graphClass.Width = width, std::forward<T>(graphClass);
	}
	template<typename T>
	std::enable_if_t<std::is_base_of_v<GraphClass, std::decay_t<T>>, T&&> MakeVisible(T&& graphClass) noexcept {
		return graphClass.Visible = true, std::forward<T>(graphClass);
	}
	template<typename T>
	std::enable_if_t<std::is_base_of_v<GraphClass, std::decay_t<T>>, T&&> MakeInvisible(T&& graphClass) noexcept {
		return graphClass.Visible = false, std::forward<T>(graphClass);
	}

	class Graph : public GraphClass {
	private:
		const Sampler* m_Sampler = nullptr;

	public:
		Graph(const Sampler* sampler, const GraphClass& graphClass) noexcept;
		Graph(const Graph&) = delete;
		virtual ~Graph() = default;

	public:
		Graph& operator=(const Graph&) = delete;

	public:
		std::vector<Line> Sample(const SamplingContext& context) const;

		Scalar Independent(const Point& point) const noexcept;
		Scalar Dependent(const Point& point) const noexcept;
		Point XY(Scalar independent, Scalar dependent) const noexcept;

		Graph* MakeForY() noexcept;
		Graph* MakeForX() noexcept;

		Graph* ChangeColor(ShitGraph::Color newColor) noexcept;
		Graph* ChangeWidth(Scalar newWidth) noexcept;
		bool MakeVisible() noexcept;
		bool MakeInvisible() noexcept;
	};
}

namespace ShitGraph {
	class Graphs final {
	private:
		std::vector<Graph*> m_Graphs;
		Point m_Center;
		Scalar m_Scale;

	public:
		Graphs(Scalar scale = 0.262144) noexcept;
		Graphs(const Graphs&) = delete;
		~Graphs() = default;

	public:
		Graphs& operator=(const Graphs&) = delete;

	public:
		Point GetCenter() const noexcept;
		void SetCenter(const Point& newCenter) noexcept;
		Scalar GetScale() const noexcept;
		void SetScale(Scalar newScale) noexcept;

		const Graph* GetGraph(std::size_t index) const noexcept;
		Graph* GetGraph(std::size_t index) noexcept;
		std::size_t GetGraphCount() const noexcept;
		void AddGraph(Graph* graph);
		void RemoveGraph(Graph* graph);
		void DeleteGraph(Graph* graph);

		void Render(GraphicDevice& device) const;

	public:
		Point Logical(int width, int height, const Point& point) const noexcept;
		Point Physical(int width, int height, const Point& point) const noexcept;

	private:
		Rectangle Logical(const GraphicDevice& device, const Rectangle& rectangle) const noexcept;
	};
}