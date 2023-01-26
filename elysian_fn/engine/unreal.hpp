#pragma once

namespace elysian_fn {
	namespace sdk {
		namespace unreal {
			template <class type>
			struct tarray {
			public:
				tarray() { this->data = nullptr; this->count = this->max = 0; };

				type* data;
				std::int32_t count, max;

				type& operator[](int i) {
					return this->data[i];
				};

				int size() {
					return this->count;
				}

				bool valid(int i) {
					return bool(i < this->count);
				}
			};

			struct fstring : private tarray<wchar_t> {
				fstring() { };
				fstring(wchar_t* other) {
					this->max = this->count = *other ? static_cast<int>(customs::wcslen(other)) + 1 : 0;

					if (this->count)
						this->data = const_cast<wchar_t*>(other);
				};

				wchar_t* c_str() {
					return this->data;
				}

				bool valid() {
					return this->data != nullptr;
				}
			};

			class uobject {
			public:
				static uobject* find_object(const wchar_t* name, uobject* outer = nullptr) {
					return reinterpret_cast<uobject*>(uobject::static_find_object(nullptr, outer, name));
				}

				static uobject* static_find_object(uobject* klass, uobject* outer, const wchar_t* name) {
					return reinterpret_cast<uobject * (*)(uobject*, uobject*, const wchar_t*)>(elysian_fn::game_base + offsets::static_find_object)(klass, outer, name);
				}

				void process_event(uobject* function, void* args) {
					return reinterpret_cast<void (*)(uobject*, uobject*, void*)>(elysian_fn::game_base + offsets::process_event)(this, function, args);
				}

				template <typename type>
				type get(uobject* member) {
					struct {
						type return_value;
					} params;

					this->process_event(member, &params);

					return params.return_value;
				}
			};
		}
	}
}

using namespace elysian_fn::sdk::unreal;