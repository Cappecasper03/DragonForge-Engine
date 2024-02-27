#ifdef TRACY_ENABLE
void* operator new( size_t _size )
{
    void* ptr = malloc( _size );
    TracyAlloc( ptr, _size );
    return ptr;
}

void operator delete( void* _ptr ) noexcept
{
    TracyFree( _ptr );
    free( _ptr );
}
#endif

int main()
{
    return 0;
}
