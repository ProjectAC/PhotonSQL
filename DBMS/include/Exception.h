#pragma once
#include <exception>

namespace Photon
{
    class TypeMismatchException : std::exception {};
    class RowLengthMismatchException : std::exception {};
    class UnknownTypeException : std::exception {};
    class ObjectNotFountException : std::exception {};
    class SQLException : std::exception {};
}