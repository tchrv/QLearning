template <typename net_type, size_t in, size_t out>
DeepQBrain<net_type, in, out>::DeepQBrain(net_type& net): m_net(net) {
	m_trainer = new dlib::dnn_trainer<net_type>(net);
}

template <typename net_type, size_t in, size_t out>
DeepQBrain<net_type, in, out>::~DeepQBrain() {
	delete m_trainer;
}

template <size_t in>
inline dlib::matrix<DQLA_FP_TYPE> state_to_matrix(ExperienceState<in>& state) {
	dlib::matrix<DQLA_FP_TYPE> m(in, 1);
	for(int i = 0; i < in; i++)
		m(i) = state[i];
	return m;
}

template <size_t out>
inline std::array<DQLA_FP_TYPE, out> matrix_to_q_array(dlib::matrix<DQLA_FP_TYPE>& mat) {
	std::array<DQLA_FP_TYPE, out> a;
	for(int i = 0; i < out; i++)
		a[i] = mat(i);
	return a;
}

template <size_t out>
inline dlib::matrix<DQLA_FP_TYPE> q_array_to_matrix(std::array<DQLA_FP_TYPE, out>& q_array) {
	dlib::matrix<DQLA_FP_TYPE> m(out, 1);
	for(int i = 0; i < out; i++)
		m(i) = q_array[i];
	return m;
}

template <typename net_type, size_t in, size_t out>
void DeepQBrain<net_type, in, out>::train_one_step(ExperienceState<in>& state, std::array<DQLA_FP_TYPE, out>& target_values) {
	std::vector<ExperienceState<in> > v(1);
	std::vector<std::array<DQLA_FP_TYPE, out> > s(1);
	v[0] = state;
	s[0] = target_values;
	train_batch(v, s);
}

template <typename net_type, size_t in, size_t out>
void DeepQBrain<net_type, in, out>::train_batch(std::vector<ExperienceState<in> >& states_vec, std::vector<std::array<DQLA_FP_TYPE, out> >& target_values_vec) {
	std::vector<typename net_type::input_type > v(states_vec.size());
	std::vector<typename net_type::training_label_type > s(target_values_vec.size());

	size_t n = states_vec.size();

	for(int i = 0; i < n; i++)
		v[i] = state_to_matrix(states_vec[i]);
	for(int i = 0; i < n; i++)
		s[i] = q_array_to_matrix(target_values_vec[i]);

	m_trainer->train_one_step(v, s);
}

template <typename net_type, size_t in, size_t out>
std::array<DQLA_FP_TYPE, out> DeepQBrain<net_type, in, out>::get_scores(ExperienceState<in>& state) {
	std::vector<ExperienceState<in> > vec(1);
	vec[0] = state;
	return get_all_scores(vec)[0];	
}

template <typename net_type, size_t in, size_t out>
std::vector <std::array<DQLA_FP_TYPE, out> > DeepQBrain<net_type, in, out>::get_all_scores(std::vector<ExperienceState<in> >& states) {
	int n = states.size();	
	std::vector<std::array<DQLA_FP_TYPE, out> > vec(n);
	std::vector<dlib::matrix<DQLA_FP_TYPE> > in_matrices(n), out_matrices(n);

	for(int i = 0; i < n; i++)
		in_matrices[i] = state_to_matrix(states[i]);
	out_matrices = m_trainer->get_net()(in_matrices);
	for(int i = 0; i < n; i++) {
		dlib::matrix<DQLA_FP_TYPE> m = out_matrices[i];
		vec[i] = matrix_to_q_array<out>(m);
	}
	return vec;
}
