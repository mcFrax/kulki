#pragma once

//przydatny szablon
//na pewno cos takiego jest gotowego, ale nie chcialo mi sie szukac

template<class T>
class Array2Accessor
{
	private:
		T* content;
		unsigned width;
	public:
		Array2Accessor(T* content, unsigned width)
			: content(content), width(width)
		{
		}
		T& operator()(unsigned x, unsigned y)
		{
			return content[y*width+x];
		}
		const T& operator()(unsigned x, unsigned y) const
		{
			return content[y*width+x];
		}
		operator T*()
		{
			return content;
		}
		operator const T*() const
		{
			return content;
		}
};

template<class T>
class Array2
{
	private:
		T* const content;
		const unsigned width;
		Array2(const Array2&); //undefined
	public:
		Array2(unsigned sizeX, unsigned sizeY)
			: content(new T[sizeX*sizeY]), width(sizeX)
		{
		}
		~Array2()
		{
			delete content;
		}
		T& operator()(unsigned x, unsigned y)
		{
			return content[y*width+x];
		}
		const T& operator()(unsigned x, unsigned y) const
		{
			return content[y*width+x];
		}
		operator T*()
		{
			return content;
		}
		operator const T*() const
		{
			return content;
		}
		operator Array2Accessor<T>()
		{
			return Array2Accessor<T>(content, width);
		}
		operator const Array2Accessor<T>() const
		{
			return Array2Accessor<T>(content, width);
		}
};