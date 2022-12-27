# This file is part of Crypt Underworld.
# 
# Crypt Underworld is free software: you can redistribute it
# and/or modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation, either
# version 3 of the License, or (at your option) any later
# version.
# 
# Crypt Underworld is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE. See the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public
# License along with Crypt Underworld. If not, see
# <https://www.gnu.org/licenses/>.
# 
# Copyright (c) 2022 Zoë Sparks <zoe@milky.flowers>

require 'nokogiri'
require 'strscan'
require 'securerandom'

class String
  ##
  # Like IO::puts, but appends the results to the string.
  def puts(str)
    if str[-1] == "\n"
      self << str
    else
      self << str + "\n"
    end
  end

  ##
  # Perform a series of sub operations based on pairs of before-after strings.
  #
  # @param pairs An array of 2-member arrays of strings with the form [before,
  #              after].
  #
  # @param selector_strat Changes how the program searches for the "before"
  #                       string. :whole_word requires the whole string to
  #                       match, :from_start requires the starting characters
  #                       to match, and :first_found (the default) works like
  #                       a normal sub with two string arguments.
  def sub_inj(pairs, selector_strat = :first_found)
    pairs.inject(self) do |result, pair|
      selector = case selector_strat
                 when :whole_word
                   /^#{pair[0]}$/
                 when :from_start
                   /^#{pair[0]}/
                 else
                   pair[0]
                 end

      result.sub(selector, pair[1])
    end
  end


  ##
  # Alternate forms of reserved words in C++ that appear in Vulkan identifiers.
  def reserved_alternates
    [
      %w(and       aand),
      %w(or        orr),
      %w(xor       xorr),
      %w(load      lowd),
      %w(inline    inlne),
      %w(default   dflt),
      %w(protected prtctd),
    ]
  end

  ##
  # Substitutes alternate forms of reserved words in C++ that appear in Vulkan
  # identifiers.
  def fix_reserved
    sub_inj(reserved_alternates, :whole_word)
  end

  ##
  # Changes the alternate forms of reserved words in C++ back to their original
  # forms.
  def unreserved
    sub_inj(reserved_alternates.map(&:reverse), :whole_word)
  end

  ##
  # An array of pairs strings relating English words for under-10 integers like
  # "three" with their equivalent Arabic numerals.
  def wordnums
    [
      %w(one   1),
      %w(two   2),
      %w(three 3),
      %w(four  4),
      %w(five  5),
      %w(six   6),
      %w(seven 7),
      %w(eight 8),
      %w(nine  9),
    ]
  end

  ##
  # Substitutes the first occurence of an English word for an under-10 integer
  # like "three" with its equivalent Arabic numeral.
  def fix_wordnums
    sub_inj(wordnums, :from_start)
  end

  ##
  # Substitutes the first occurence of an Arabic numeral with its equivalent
  # English word.
  def fix_startnums
    sub_inj(wordnums.map(&:reverse), :from_start)
  end

  ##
  # Converts any abbreviation for dimensions 1–3 with a lowercase "d", such as
  # "2d," to one with a capital "D," such as "2D."
  def fix_dimdowncase
    sub_inj((1..3).map { %W( #{_1}d #{_1}D ) })
  end

  ##
  # Splits the string on underscores and abbreviates each substring, in a manner
  # designed to create convenient-yet-readable names for functions and enum
  # values and the like.
  def abbrev
    split('_').map do |str|
      if (['r', 'g', 'b', 'a']
        .permutation
        .map { /^#{_1}\d+#{_2}\d+#{_3}\d+#{_4}\d+$/.match?  str }
        .inject(:|)) == true

        next str
      end

      if str == 'one6'
        str = 'sixteen'
      elsif str == 'three2'
        str = 'thirtytwo'
      elsif str == 'six4'
        str = 'sixtyfour'
      elsif str == 'image'
        str = 'img'
      end

      str_sans_nums = str.delete((1..9).to_a.join)
      if str_sans_nums == 'float'
        next str
      end

      length_sans_nums = str_sans_nums.length

      if length_sans_nums <= 3
        next str
      end

      fst = str[0]
      mid = str[1..(str.length - 2)]
      last = str[str.length - 1]

      if length_sans_nums > 6
        mid.gsub!(/[aeiou]/, '')
      end

      pre = fst + mid + last

      if pre.length > 6
        pre = pre[0,6]
      end

      %w(t d s n y).each do |l|
        if str[-1] == l && pre[-1] != l
          pre += l
        end
      end

      if str[-2, 2] == "le" && !(['e', 'l'].include? pre[-1])
        pre += 'l'
      end

      if str[-2, 2] == "wo" && pre[-1] != 'w'
        pre += 'w'
      end

      pre
    end.join('_')
  end
end

##
# If you include this module in a class, you will be able to get
#
# * a list of all the instance methods you've defined in the class
#     * after the point where you included this module
# * in the order you added them
# * using the instance method inst_mthds_in_order.
module OrderedInstMethds
  def self.included(klass)
    klass.class_eval %Q{
      @@inst_mthds_in_order = []

      def inst_mthds_in_order
        @@inst_mthds_in_order
      end

      def self.method_added(name)
        @@inst_mthds_in_order << name
      end
    }
  end
end

##
# A fairly abstract representation of the definition of some C++ construct
# relating to a type with "members," such as an enum class.
#
# If you inherit from CPPMembersDefn, it expects that every instance method
# you define in the subclass will return a string. The subclass's to_s method
# will then return a single string made by calling each of the subclass's
# instance methods in the order you defined them. See the existing subclasses of
# CPPMembersDefn for examples.
class CPPMembersDefn
  def self.inherited(klass)
    klass.include(OrderedInstMethds)
    # this is convenient, but it does mean that every instance method of the
    # subclass must produce a component of the source string
    #
    # if that becomes a problem, an alternative is to have the subclass include
    # OrderedInstMethds after any instance methods that should be left out, like
    # so:
    #
    #     class A
    #       def non
    #       end
    #
    #       include OrderedInstMethds
    #
    #       def a
    #       end
    #
    #       def b
    #       end
    #
    #       def c
    #       end
    #     end
    #
    #     puts A.new.inst_mthds_in_order
    #     #=> a
    #         b
    #         c
    #
  end

  attr_reader :name, :val_names
  def initialize(name, val_names)
    @name      = name
    @val_names = val_names
  end

  def to_s
    inst_mthds_in_order.inject("") {|res, methd| res.puts send(methd) }
  end
end

##
# The definition of a C++ enum class.
class CPPEnumClassDefn < CPPMembersDefn
  def opening
    <<~END
    /*!
     * \\brief Maps to #{name.name}. Should be safe to static_cast between.
     */
    enum class #{name.no_vk} {
    END
  end

  def val_entries
    val_names.map do |new_name|
      unless new_name.bad?
        "    #{new_name.abbrev} = #{new_name.old_name},"
      end
    end.join("\n")
  end

  def closer
    "};\n\n"
  end
end

##
# The definition of a function to convert members of a Vulkan-type-wrapping enum
# class to easily-readable strings, including an overload for values of the
# Vulkan type.
class CPPEnumStringConvDefn < CPPMembersDefn
  def opening
    <<~END
    #{name.str_fn_sig}(#{name.no_vk} val)
    {
        using enum #{name.no_vk};

        switch(val) {
    END
  end

  def cases
    out = ""
    val_names.each do |new_name|
      unless new_name.bad?
        out.puts "    case #{new_name.abbrev}:"
        out.puts "        return \"#{new_name.loggable}\";"
      end
    end
    out
  end

  def closer
    <<~END
        default:
            return std::to_string(static_cast<int>(val));
        }
    }

    END
  end

  def vk_overload
    <<~END
    #{name.str_fn_sig}(#{name.name} val)
    {
        return #{name.str_fn_name}(static_cast<#{name.no_vk}>(val));
    }

    END
  end
end

##
# The definition of a function to convert a Vulkan-bitmask-wrapping
# enum class object to a vector of log-friendly C-style strings describing each
# enabled flag.
class CPPBitflagsCstrsConvDefn < CPPMembersDefn
  def opening
    <<~END
    constexpr std::vector<const char*> #{name.cstrs_fn_name}(#{name.gsub('Bits', 's')} vals)
    {
        std::vector<const char*> cstrs;

    END
  end

  def cases
    out = ""
    val_names.each do |new_name|
      unless new_name.bad?
        out.puts "    if (vals & #{new_name.old_name}) {"
        out.puts "        cstrs.push_back(\"#{new_name.loggable}\");"
        out.puts "    }\n\n"
      end
    end
    out
  end

  def closer
    <<~END
        return cstrs;
    }

    END
  end
end

##
# A fairly abstract class representing a Vulkan enum identifier.
class EnumClassName
  attr_reader :name

  ##
  # (constructor)
  #
  # @param name The Vulkan enum identifier, such as `VkCompareOp`.
  def initialize(name)
    @name = name
  end

  ##
  # The identifier without `KHR` or other similar suffixes.
  def no_suffix
    name.sub('KHR', '')
  end

  ##
  # The identifier without the `Vk` prefix or any common Vulkan suffixes; the
  # "plain" name of the Vulkan enum.
  def no_vk
    no_suffix.sub('Vk', '')
  end

  ##
  # The opening part of the all-caps-with-underscores names of values of the
  # Vulkan enum, such as `VK_COMPARE_OP_`.
  def prename
    s = StringScanner.new(no_suffix)
    match = ""
    prename = ""
    loop do
      match = s.scan_until((/[a-z](?=[A-Z])/))
      if match
        prename += match + '_'
      else
        break
      end
    end
    prename += s.scan_until(/$/) + '_'
    prename.upcase
  end

  ##
  # The prename, in lowercase and without `VK_`. Used as the unabbreviated
  # starting point for our names of functions relating to the enum and the like.
  def fn_prename
    prename.sub('VK_', '').downcase
  end

  ##
  # The name of our string conversion function for values of this enum.
  def str_fn_name(inter="")
    "#{fn_prename.abbrev}#{inter}_str"
  end

  ##
  # The signature of our string conversion function for values of this enum.
  def str_fn_sig
    "constexpr std::string #{str_fn_name}"
  end

  ##
  # Redirects unknown methods to the Vulkan identifier.
  def method_missing(meth, *args)
    if @name.respond_to? meth
      @name.public_send(meth, *args)
    else
      super
    end
  end

  ##
  # The name of our function that converts a bitflags value using members of
  # this enum to a vector of C strings representing each enabled flag.
  def cstrs_fn_name
    "#{fn_prename.abbrev}_flags_cstrs"
  end
end

##
# The name of a Vulkan enum.
class EnumName < EnumClassName
  def str_fn_name
    super
  end
end

##
# The name of a Vulkan bitmask.
class BitmaskName < EnumClassName
  def no_suffix
    super.sub('FlagBits', '')
  end

  def no_vk
    super + 'Flag'
  end

  def str_fn_name
    super("_flag")
  end
end

##
# A fairly abstract class representing the name of a member of a Vulkan enum
# type.
class EnumClassValName
  attr_reader :old_name, :name, :value, :enum_class_name

  ##
  # (constructor)
  #
  # @param old_name The member's Vulkan name, such as `VK_COMPARE_OP_EQUAL`.
  # @param value    The value Vulkan assigns to this member within its enum.
  # @param name     An object supporting the EnumClassName interface
  #                 representing the name of this member's enum.
  def initialize(old_name, value, enum_class_name)
    @old_name        = old_name
    @value           = value
    @enum_class_name = enum_class_name
    @name            = new_name
  end

  ##
  # Our name for this member.
  def new_name
    old_name
      .sub(enum_class_name.prename, '')
      .sub(enum_class_name.fn_prename.upcase.gsub('_', ''), '')
      .sub('_KHR', '')
      .sub('VK_', '')
      .downcase
      .then(&method(:mod_new_name))
      .fix_startnums
      .fix_reserved
      .strip
  end

  ##
  # Subclasses can override this to provide additional tranformations for the
  # conversion from the old name to the new name.
  def mod_new_name(n)
    n
  end

  ##
  # A log-worthy, print-friendly version of new_name.
  def loggable
    name
      .gsub('_', ' ')
      .downcase
      .sub("vk  ", '')
      .fix_wordnums
      .fix_dimdowncase
      .unreserved
      .sub_inj([
        %w(fifo FIFO),
        %w(srgb sRGB),
        ['depth stencil', 'depth/stencil'],
      ])
  end

  ##
  # If true, we leave this member out of our version of the enum.
  def bad?
    (/^_/ =~ new_name) \
      || (/_ext$/ =~ new_name) \
      || (new_name.include? 'query_scope')
  end

  ##
  # Forwards incomprehensible messages to name.
  def method_missing(meth, *args)
    if @name.respond_to? meth
      @name.public_send(meth, *args)
    else
      super
    end
  end
end

##
# The name of a Vulkan enum member.
class EnumValName < EnumClassValName
  def mod_new_name(n)
    super
      .sub_inj([
        %w(1d oneD),
        %w(2d twoD),
        %w(3d threeD),
        %w(fifo FIFO),
      ])
  end
end

##
# The name of a Vulkan bitmask flag.
class BitmaskValName < EnumClassValName
  def mod_new_name(n)
    super
      .sub(/_bit$/, '')
  end
end

##
# A fairly abstract class representing a C++ enum class corresponding to a
# Vulkan enum or bitmask.
class EnumClass
  attr_reader :name, :val_names

  ##
  # (constructor)
  #
  # @param name   Vulkan's name for this type.
  # @param vk_xml The Nokogiri-wrapped contents of Vulkan's `vk.xml`.
  def initialize(name, vk_xml)
    @name           = name_class.new(name)
    @val_names      = new_val_names(vk_xml)
  end

  ##
  # Methods that generate C++ definitions relating to the enum class.
  [
    [:defn,           CPPEnumClassDefn],
    [:string_conv_fn, CPPEnumStringConvDefn],
    [:cstrs_fn,       CPPBitflagsCstrsConvDefn],
  ].each do |meth_name, defn_class|
    define_method(meth_name) do
      defn_class.new(name, val_names).to_s
    end
  end
  
  ##
  # The type to represent the name of the enum class.
  #
  # Designed to work gracefully with subclasses. If you name your subclass
  # Bitmask, name_class will return a class named BitmaskVal. This class
  # expects the returned class (i.e. BitmaskVal) to support the interface of
  # EnumClassName.
  def name_class
    Object.const_get(self.class.to_s + "Name")
  end

  ##
  # The type to represent the names of the members of the enum class.
  #
  # Designed to work gracefully with subclasses. If you name your subclass
  # Bitmask, name_class will return a class named BitmaskValName. This
  # class expects the returned class (i.e. BitmaskValName) to support the
  # interface of EnumClassValName.
  def val_name_class
    Object.const_get(self.class.to_s + "ValName")
  end

  ##
  # Subclasses can override this to apply extra CSS selectors to the Vulkan enum
  # type selector used in new_val_names. See Bitmask for an example.
  def extra_selectors
    ''
  end

  ##
  # Maps the names of the Vulkan enum members found in vk_xml to an array of
  # unique val_name_class objects.
  #
  # @param vk_xml The Nokogiri-wrapped contents of Vulkan's `vk.xml`.
  def new_val_names(vk_xml)
    vk_xml.css(
      "enums[name=#{name.name}]#{extra_selectors} > enum:not([alias])"
    ).then do |els|
      %w(1.0 1.1 1.2 1.3).each do |vk_ver|
        els += vk_xml.css(
          "feature[api=vulkan][number='#{vk_ver}'] " \
          + "enum[extends=#{name.name}]:not([alias])"
        )
      end

      els
    end.map do |val|
      val_name_class.new(
        val['name'],
        val['value'] ? val['value'] : val['bitpos'],
        name,
      )
    end.uniq(&:new_name)
       .uniq(&:old_name)
  end

  ##
  # Whether or not this enum class actually has any members.
  def empty?
    val_names.empty?
  end

  ##
  # The complete C++ source text for our enum class.
  def source(extra = "")
    if empty?
      ""
    else
      defn + string_conv_fn + extra
    end
  end

  ##
  # A string that can be used to find the Vulkan entry for this type in `vk.xml`
  # using the selector `enums[type=#{specifier}]`.
  def self.specifier
    to_s.downcase
  end
end

##
# Our enum class corresponding to a Vulkan enum.
class Enum < EnumClass
  def source
    super
  end
end

##
# Our enum class corresponding to a Vulkan bitmask.
class Bitmask < EnumClass
  def source
    super(cstrs_fn)
  end
end

##
# Generates a C++ header wrapping Vulkan's enum and bitflag types, with
# convenient string conversion functions for each.
class VulkanUtilHeader
  PREAMBLE = <<~END.lstrip
  /*
   * This file is part of Crypt Underworld.
   *
   * Crypt Underworld is free software: you can redistribute it
   * and/or modify it under the terms of the GNU General Public
   * License as published by the Free Software Foundation, either
   * version 3 of the License, or (at your option) any later
   * version.
   *
   * Crypt Underworld is distributed in the hope that it will be
   * useful, but WITHOUT ANY WARRANTY; without even the implied
   * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
   * PURPOSE. See the GNU General Public License for more details.
   *
   * You should have received a copy of the GNU General Public
   * License along with Crypt Underworld. If not, see
   * <https://www.gnu.org/licenses/>.
   *
   * Copyright (c) 2022 Zoë Sparks <zoe@milky.flowers>
   */

  // This file was generated by #{File.basename(__FILE__, '_lib.rb')} in #{File.basename(__dir__)}.
  //
  // If you need to modify this file, please edit that script rather than
  // changing it by hand.

  #ifndef dca51d742abb4a1e937d117355829212
  #define dca51d742abb4a1e937d117355829212

  #include <vulkan/vulkan.h>

  #include <string>
  #include <vector>
  #include <algorithm>

  namespace cu {
  namespace vk {

  END

  POSTAMBLE = <<~END.rstrip

  } // namespace vk
  } // namespace cu

  #endif
  END

  def initialize(vk_xml_path)
    @out = ""
    @vk_xml = get_vk_xml(vk_xml_path)
  end

  ##
  # Parses `vk.xml`.
  def get_vk_xml(vk_xml_path)
    vk_xml_txt =
      begin
        File.open(vk_xml_path, 'r', &:read)
      rescue
        puts "failed to open vk.xml at path #{vk_xml_path}"
        exit 1
      end

    Nokogiri::XML(vk_xml_txt)
  end

  ##
  # Filters out Vulkan types that we shouldn't wrap, like its aliases for
  # built-in C types or types that come from non-`KHR` extensions.
  def vk_names_filter(names)
    names.delete("API Constants")

    %w(
      EXT
      NV
      INTEL
      AMD
      FUCHSIA
      ANDROID
      QueryResult
      Video
      FlagBits2
    ).each do |n|
      names.delete_if { _1.include? n }
    end

    names
  end

  ##
  # Gets the names of all the Vulkan types corresponding to type.
  #
  # @param type An identifier of a Vulkan enum type, such as `enum` or
  #             `bitmask`.
  def get_vk_names(type)
    vk_names_filter(@vk_xml.css("enums[type=#{type}]").map { _1["name"] })
  end

  ##
  # Generates and returns the header.
  def generate
    @out = ""

    @out.puts PREAMBLE

    enum_class_types = [Enum, Bitmask]

    enum_class_types.each do |enum_class_type|
      get_vk_names(enum_class_type.specifier).each do |name|
        source = enum_class_type.new(name, @vk_xml).source

        unless source.empty?
          @out.puts source
        end
      end
    end

    @out.puts POSTAMBLE

    @out
  end
end
