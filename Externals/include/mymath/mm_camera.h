#ifndef mm_camera_h
#define mm_camera_h

namespace mymath
{
  template< typename t >
  class camera
  {
    private:

    protected:

    public:
      impl::vec3i<t> pos;
      impl::vec3i<t> view_dir;
      impl::vec3i<t> up_vector;

      void lookat( const impl::vec3i<t>& eye, const impl::vec3i<t>& lookat, const impl::vec3i<t>& up )
      {
        view_dir = normalize( lookat - eye );
        up_vector = normalize( up );
        pos = eye;
        impl::vec3i<t> right = cross( view_dir, up_vector );
        up_vector = cross( right, view_dir );
      }

      void translate( const impl::vec3i<t>& vec )
      {
        pos += vec;
      }

      void move_forward( const t& d )
      {
        pos += view_dir * impl::vec3i<t>( d );
      }

      void move_up( const t& d )
      {
        pos += up_vector * impl::vec3i<t>( d );
      }

      void move_right( const t& d )
      {
        vec3 x = cross( view_dir, up_vector );
        pos += x * impl::vec3i<t>( d );
      }

      void rotate_x( const t& angle )
      {
        impl::mat3i<t> rot_mat;
        impl::vec3i<t> x_vec;

        x_vec = cross( view_dir, up_vector );
        rot_mat = impl::mat3i<t>( create_rotation( angle, x_vec ) );

        up_vector = rot_mat * up_vector;
        view_dir = rot_mat * view_dir;
      }

      void rotate_y( const t& angle )
      {
        impl::mat4i<t> rot_mat = create_rotation( angle, up_vector );

        view_dir = impl::mat3i<t>( rot_mat ) * view_dir;
      }

      void rotate_z( const t& angle )
      {
        impl::mat4i<t> rot_mat = create_rotation( angle, view_dir );

        up_vector = impl::mat3i<t>( rot_mat ) * up_vector;
      }

      void rotate( const t& angle, const impl::vec3i<t>& vec )
      {
        impl::mat3i<t> rot_mat = impl::mat3i<t>( create_rotation( angle, vec ) );

        up_vector = rot_mat * up_vector;
        view_dir = rot_mat * view_dir;
      }

      impl::mat4i<t> get_matrix() const
      {
        impl::vec3i<t> x = cross( view_dir, up_vector );

        impl::mat4i<t> m;
        m[0] = vec4( x, 0 );
        m[1] = vec4( up_vector, 0 );
        m[2] = vec4( -view_dir, 0 );
        m[3] = vec4( vec3(0), 1 );
        m = transpose( m );

        return m * create_translation(-pos);
      }

      void set_from_matrix( const impl::mat4i<t>& m )
      {
        impl::mat4i<t> inv = inverse( m );
        pos = inv[3].xyz;
        up_vector = m[1].xyz;
        view_dir = m[2].xyz;
      }

      camera() : pos( impl::vec3i<t>( 0, 0, 0 ) ), view_dir( impl::vec3i<t>( 0, 0, -1 ) ), up_vector( impl::vec3i<t>( 0, 1, 0 ) ) {}

  };
}

#endif
