#pragma once

#include "Global.hpp"

#include <vector>
#include <Utility/Meta.hpp>

namespace TG
{
    namespace Parser
    {
        using std::vector;

        template <typename TypesMeta>
        class Leaf
        {
        private:
            using TokenReaderIndex = typename TypesMeta::TokenReaderIndex;
            using Index = typename TypesMeta::Index;

            enum eMetaType { Operator, Value };

            eMetaType metaType{Operator};
            TokenReaderIndex tokenReaderIndex = TokenReaderIndex(-1);
            Index previousPeerIndex = Index(-1);
            Index data1;
            Index data2;

        public:
            inline TokenReaderIndex getTokenReaderIndex() const noexcept { return tokenReaderIndex; }
            inline Index getPreviousPeerIndex() const noexcept { return previousPeerIndex; }

            inline Index& operatorDataChildrenCount() noexcept { return data1; }
            inline Index& operatorDataLastChild() noexcept { return data2; }

            inline Index& valueDataStart() noexcept { return data1; }
            inline Index& valueDataLength() noexcept { return data2; }

            inline Index childrenCount() const noexcept
            {
                return metaType == Value ?
                    0 :
                    data1;
            }
        };

        template <typename Iterator,
                  typename TypesMeta>
        class Token
        {
        public:
            enum PlaceAs { Discard, Value, Operator };
            bool isRead = false;
            PlaceAs placeAs = Discard;
            Iterator begin;
            Leaf<TypesMeta> leaf;
            
            static inline
            Token check(Iterator const& begin,
                        Iterator const& end,
                        char const* str) noexcept
            {
                Token<Iterator, TypesMeta> token;
                token.isRead = false;
                Iterator copy = begin;

                for (; copy != end && *str != '\0'; ++copy, ++str)
                {
                    if (*copy != *str)
                        return token;
                }

                if (*str == '\0')
                    token.begin = copy;
                else
                    token.begin = begin;

                token.isRead = true;

                return token;
            }
        };

        template <typename Iterator,
                  typename TypesMeta>
        using TokenReaderFunction = Token<Iterator, TypesMeta> (*)(Iterator const&,
                                                                   Iterator const&) noexcept;

        template <typename TypesMeta>
        class Tree
        {
        private:
            vector<Leaf<TypesMeta>> flatTree;
            vector<char> rawExpression;
        };
    } // namepsace Parser

    namespace ParserDetail
    {
        using TG::Parser::Token;
        using TG::Parser::TokenReaderFunction;

        template <typename Iterator,
                  typename TypesMeta,
                  typename TokenReader>
        class TokenReaderHelperNested
        {
        public:
            template <typename T,
                      TokenReaderFunction<Iterator, TypesMeta> = &T::readOperator>
            static inline
            Token<Iterator, TypesMeta> readToken(Iterator const& begin,
                                                 Iterator const& end,
                                                 T*) noexcept
            {
                auto token = T::template readOperator<Iterator, TypesMeta>(begin, end);

                if (token.isRead)
                {
                    //assert(token.begin != begin);

                    // Token.leaf = Leaf<TypesMeta>(T_operator_lexer::rtt,
                    //                              T_operator_lexer::priority);
                }

                return token;
            }

            template <typename T = TokenReader>
            static inline
            Token<Iterator, TypesMeta> readToken(Iterator const& begin,
                                                 Iterator const& end,
                                                 ...) noexcept
            {
                auto token = T::template readDiscard<Iterator, TypesMeta>(begin, end);

                if (token.isRead)
                    token.placeAs = Token<Iterator, TypesMeta>::Discard;

                return token;
            }
        };

        template <typename Iterator,
                  typename TypesMeta,
                  typename TokenReader>
        class TokenReaderHelper
        {
        public:
            template <typename T,
                      TokenReaderFunction<Iterator, TypesMeta> = &T::readValue>
            static inline
            Token<Iterator, TypesMeta> readToken(Iterator const& begin,
                                                 Iterator const& end,
                                                 T*) noexcept
            {
                auto token = T::template readValue<Iterator, TypesMeta>(begin, end);

                if (token.isRead)
                {
                    token.placeAs = Token<Iterator, TypesMeta>::Value;
                    //assert(token.begin != begin);

                    // token.leaf = Leaf<TypesMeta>(T_value_lexer::rtt,
                    //                        it_copy - it_begin);
                }

                return token;
            }

            template <typename T = TokenReader>
            static inline
            Token<Iterator, TypesMeta> readToken(Iterator const& begin,
                                                 Iterator const& end,
                                                 ...) noexcept
            {
                return TokenReaderHelperNested<Iterator, TypesMeta, TokenReader>::template readToken(begin,
                                                                                                     end,
                                                                                                     static_cast<T*>(nullptr));
            }
        };

        //

        using TG::Meta::TypeList;

        template <typename Iterator,
                  typename TypesMeta,
                  typename...>
        struct Lexer;

        template <typename Iterator,
                  typename TypesMeta,
                  typename TokenReader,
                  typename... TokenReaders>
        struct Lexer<Iterator, TypesMeta, TypeList<TokenReader, TokenReaders...>>
        {
            static inline
            Token<Iterator, TypesMeta> getToken(Iterator const& begin,
                                                Iterator const& end) noexcept
            {
                auto token = TokenReaderHelper<Iterator, TypesMeta, TokenReader>::template readToken(begin, end, static_cast<TokenReader*>(nullptr));
                if (false == token.isRead)
                    token = Lexer<Iterator, TypesMeta, TypeList<TokenReaders...>>::getToken(begin, end);
                
                return token;
            }
        };
        template <typename Iterator,
                typename TypesMeta,
                typename TokenReader>
        struct Lexer<Iterator, TypesMeta, TypeList<TokenReader>>
        {
            static inline
            Token<Iterator, TypesMeta> getToken(Iterator const& begin,
                                                Iterator const& end) noexcept
            {
                return TokenReaderHelper<Iterator, TypesMeta, TokenReader>::template readToken(begin, end, static_cast<TokenReader*>(nullptr));
            }
        };
    } // namepsace ParserDetail
} // namespace TG