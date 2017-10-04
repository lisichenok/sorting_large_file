#pragma once

class Segment
{
public:
    Segment(size_t beg = 0, size_t end = 0)
        :beg(beg), end(end)
    {
        if(end < beg)
        {
            end = beg = 0;
        }
    }

    size_t size()
    {
        return end - beg;
    };

    size_t beg;
    size_t end;
};

