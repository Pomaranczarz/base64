#include <array>
#include <cmath>
#include <iostream>
#include <vector>

class Base64 {
public:
	Base64() = delete;

	/// Encode string with base64
	[[nodiscard]] static std::string encode(std::string s) noexcept
	{
		while (s.size() % 3 != 0)	   // fill with padding if not divisible by 3
			s.push_back(Pad);

		auto binaryString = stringToBinary(s);	    // convert character string into corresponding vector of bits
		std::string result = "";

		int value = 0;
		for (size_t i = 0, j = 0; i < binaryString.size(); i++, j++) {	    // take 6 bits and interpret them as a character value from base64 table
			if (j != 0 && j % 6 == 0) {
				result.push_back(Codes[value]);
				value = 0;
				j = 0;
			}

			value += binaryString[i] * (int) std::pow(2, 5 - j);
		}

		return result;
	}

	/// Decode base64 encoded string
	static std::string decode(const std::string& s)
	{
		auto charactersCodes = stringToCodes(s);
		auto codesBits = codesToBits(charactersCodes);
		std::string result = "";

		int value = 0;
		for (size_t i = 0, j = 0; i < codesBits.size(); i++, j++) {		 // take 8 bits and interpret them as 8-bit character
			if (j != 0 && j % 8 == 0) {
				result.push_back(value);
				value = 0;
				j = 0;
			}

			value += codesBits[i] * (int) std::pow(2, 7 - j);
		}

		return result;
	}

private:
	static constexpr char Pad{ '=' };
	static constexpr std::string_view Codes{ "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" };

	/// Convert any type T into its binary representation
	template <typename T>
	[[nodiscard]] static std::vector<uint8_t> toBits(T value) noexcept
	{
		std::vector<uint8_t> bits;
		const int shift = 8 * sizeof(T) - 1;
		const unsigned mask = 1 << shift;

		for (int i = 0; i < shift + 1; i++) {	   // get rightmost bit and shift number to the left
			bits.push_back((value & mask) ? 1 : 0);
			value <<= 1;
		}

		return bits;
	}

	/// Convert string to its binary represenatation
	[[nodiscard]] static std::vector<uint8_t> stringToBinary(const std::string& s) noexcept
	{
		std::vector<uint8_t> result;
		for (auto c : s) {	  // turn each character into its bit representation and merge together
			auto bits = toBits(c);
			result.insert(result.end(), bits.begin(), bits.end());
		}

		return result;
	}

	/// Get character's base64 code
	[[nodiscard]] static uint8_t getCodeFromBase64Character(char c)
	{
		for (size_t i = 0; i < Codes.size(); i++)		 // index of given character is its code
			if (c == Codes[i])
				return i;

		return -1;
	}

	/// Convert base64 encoded string into string of characters' codes
	[[nodiscard]] static std::vector<uint8_t> stringToCodes(std::string s)
	{
		std::vector<uint8_t> result;
		for (auto c : s)
			result.push_back(getCodeFromBase64Character(c));

		return result;
	}

	/// Convert vector of codes into vector of corresponding binary values
	[[nodiscard]] static std::vector<uint8_t> codesToBits(std::vector<uint8_t> codes)
	{
		std::vector<uint8_t> bits;
		for (auto code : codes) {
			auto codeBits = toBits(code);
			for (size_t i = 2; i < codeBits.size(); i++)	    // take only 6 bits as the remaining 2 are there to fill to 8 bits and disturb calculations
				bits.push_back(codeBits[i]);
		}

		return bits;
	}
};

int main()
{
	std::string input = "Idealy sa jak gwiazdy - nie mozna ich osiagnac, ale mozna sie nimi kierowac.";
	std::string encoded = Base64::encode(input);
	std::string decoded = Base64::decode(encoded);
	std::cout << encoded << '\n';
	std::cout << decoded << '\n';
}
