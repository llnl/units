/*
Copyright (c) 2019-2025,
Lawrence Livermore National Security, LLC;
See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include "units_decl.hpp"

#include <cstdint>
#include <cstdlib>

namespace UNITS_NAMESPACE {

namespace commodities {
    /// generate a harmonized code for use with the units library based on the
    /// international harmonized code for international trade

    constexpr std::uint32_t generateHarmonizedCode(
        std::uint32_t chapter,
        std::uint32_t section,
        std::uint32_t subsection)
    {
        return ((chapter % 100U) << 14U) + ((section % 100U) << 7U) +
            ((subsection % 100U));
    }

    constexpr std::uint32_t generateHarmonizedCode(
        std::uint32_t chapter,
        std::uint32_t section,
        std::uint32_t subsection,
        std::uint32_t code1,
        bool digit = false)
    {
        return ((chapter % 100U) << 14U) + ((section % 100U) << 7U) +
            ((subsection % 100U)) + ((code1 % 100U) << 21U) +
            ((digit) ? 0x1000000U : 0U);
    }

    // NOLINTBEGIN(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    template<size_t N>
    constexpr std::uint32_t generateStringCode(const char (&code)[N])
    {
        static_assert(N == 6, "invalid string code, must be 5 characters");
        return 0x60000000U +
            ((static_cast<std::uint32_t>(code[0] - '_') & 0X1FU) << 20U) +
            ((static_cast<std::uint32_t>(code[1] - '_') & 0X1FU) << 15U) +
            ((static_cast<std::uint32_t>(code[2] - '_') & 0X1FU) << 10U) +
            ((static_cast<std::uint32_t>(code[3] - '_') & 0X1FU) << 5U) +
            (static_cast<std::uint32_t>(code[4] - '_') & 0X1FU);
    }

    template<size_t N>
    constexpr std::uint32_t generateStringCodeUpper(const char (&code)[N])
    {
        static_assert(N == 6, "invalid string code, must be 5 characters");
        return 0x70000000U +
            ((static_cast<std::uint32_t>(code[0] - '@') & 0X1FU) << 20U) +
            ((static_cast<std::uint32_t>(code[1] - '@') & 0X1FU) << 15U) +
            ((static_cast<std::uint32_t>(code[2] - '@') & 0X1FU) << 10U) +
            ((static_cast<std::uint32_t>(code[3] - '@') & 0X1FU) << 5U) +
            (static_cast<std::uint32_t>(code[4] - '@') & 0X1FU);
    }

    template<size_t N>
    constexpr std::uint32_t generateShareCode(const char (&code)[N])
    {
        return generateStringCodeUpper(code) + (1U << 25U);
    }

    // NOLINTEND(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)

    constexpr std::uint32_t generateKnownCode(std::uint32_t code)
    {
        return 0x5C000000U + (code & 0x03FFFFFFU);
    }

    constexpr std::uint32_t generatePackagingCode(std::uint32_t code)
    {
        return 0x20000000U + ((code & 0xFFU) << 21U);
    }

    constexpr std::uint32_t generatePackagingCodeAlternate(std::uint32_t code)
    {
        return 0x20000000U + (((code & 0x7FU) + 0x80U) << 21U);
    }

    // NOLINTBEGIN(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)

    template<size_t N>
    constexpr std::uint32_t generateCurrencyCode(const char (&code)[N])
    {
        static_assert(N == 4, "invalid currency code, must be 3 characters");
        return 0x46000000U + (static_cast<std::uint32_t>(code[0]) << 16U) +
            (static_cast<std::uint32_t>(code[1]) << 8U) +
            static_cast<std::uint32_t>(code[2]);
    }

    template<size_t N>
    constexpr std::uint32_t generateChemCode(const char (&code)[N])
    {
        static_assert(N == 5, "invalid chem code must be 4 characters");
        return 0x4D000000U +
            ((static_cast<std::uint32_t>(code[0] - ' ') & 0X3FU) << 18U) +
            ((static_cast<std::uint32_t>(code[1] - ' ') & 0X3FU) << 12U) +
            ((static_cast<std::uint32_t>(code[2] - ' ') & 0X3FU) << 6U) +
            (static_cast<std::uint32_t>(code[3] - ' ') & 0X3FU);
    }
    // NOLINTEND(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)

    namespace packaging {
        /** Enumeration of potential packaging/form factor modifiers
        aligned with recommendation 21 of international trade to the extent
        possible
        */
        enum packaging : std::uint32_t {
            // bulk 0-9
            bulk = generatePackagingCode(0),
            powder = generatePackagingCode(1),
            grain = generatePackagingCode(2),
            nodule = generatePackagingCode(3),
            liquid = generatePackagingCode(4),
            gas_liquid = generatePackagingCode(5),
            gas = generatePackagingCode(6),

            // wet or dry (typically volume or mass)

            dry = generatePackagingCode(7),
            dry90 = generatePackagingCode(8),
            wet = generatePackagingCode(9),

            // loose,unpacked 10-19
            loose = generatePackagingCode(10),
            tube = generatePackagingCode(11),
            rod = generatePackagingCode(12),
            roll = generatePackagingCode(13),
            coil = generatePackagingCode(14),
            sheet = generatePackagingCode(15),
            bar = generatePackagingCode(16),
            ingot = generatePackagingCode(17),
            slipsheet = generatePackagingCode(18),
            flake = generatePackagingCode(19),
            briquet = generatePackagingCode(20),
            // rigid box type
            match_box = generatePackagingCode(21),
            can = generatePackagingCode(22),
            carton = generatePackagingCode(23),
            crate = generatePackagingCode(24),
            chest = generatePackagingCode(25),
            cage = generatePackagingCode(26),
            basket = generatePackagingCode(27),
            tank = generatePackagingCode(28),
            bottlecrate = generatePackagingCode(29),
            
            // rigid drum 30-39
            tub = generatePackagingCode(30),
            ampoule = generatePackagingCode(31),
            bottle = generatePackagingCode(32),
            jerrycan = generatePackagingCode(33),
            drum = generatePackagingCode(34),
            vat = generatePackagingCode(35),
            //36
            //37
            cylindrical_tank = generatePackagingCode(38),
            //39
            // rigid bulb 40-49
            //40
            jug = generatePackagingCode(41),
            bulbous_bottle = generatePackagingCode(42),
            carboy = generatePackagingCode(43),
            barrel = generatePackagingCode(44),
            large_barrel = generatePackagingCode(45),
            keg = generatePackagingCode(46),
            jar =generatePackagingCode(47),
            //48
            // 49
            // rigid other 50-59
            //50
            bucket = generatePackagingCode(51),
            coffin = generatePackagingCode(54),
            base_box = generatePackagingCode(55),
            // flexible bagtype 60-69
            sachet = generatePackagingCode(61),
            bag_small = generatePackagingCode(62),
            bag_medium = generatePackagingCode(63),
            bag_large = generatePackagingCode(64),
            bale = generatePackagingCode(65),
            net = generatePackagingCode(66),
            filmpack = generatePackagingCode(67),
            pouch = generatePackagingCode(68),
            // 70-79 reserved
            displacement = generatePackagingCode(70),
            lump = generatePackagingCode(71),
            linear = generatePackagingCode(72),
            estimated=generatePackagingCode(73),
            // 74
            // units library is using 75-89 as descriptors
            minimum = generatePackagingCode(75),
            nominal = generatePackagingCode(76),
            maximum = generatePackagingCode(77),
            average = generatePackagingCode(78),
            topical = generatePackagingCode(79),
            //
            actual = generatePackagingCode(80),
            theoretical = generatePackagingCode(81),
            net_quantity = generatePackagingCode(82),
            gross_quantity = generatePackagingCode(83),
            fixed_rate = generatePackagingCode(84),
            deadweight = generatePackagingCode(85),
            gross_register = generatePackagingCode(86),
            net_register = generatePackagingCode(87),
            freight = generatePackagingCode(88),
            capacity = generatePackagingCode(89),
            // other special packages
            bobbin = generatePackagingCode(91),
            jumbo = generatePackagingCode(92),
            cake = generatePackagingCode(93),
            excess = generatePackagingCode(94),
            guage = generatePackagingCode(95),
            equivalent = generatePackagingCode(96),
            pad = generatePackagingCode(97),
            tray = generatePackagingCode(98),
            // the remaining are not standard containers
            // 100 - 109 vehicles
            vehicle = generatePackagingCode(100),
            car = generatePackagingCode(101),
            locomotive = generatePackagingCode(102),
            caboose = generatePackagingCode(103),
            train = generatePackagingCode(104),
            truck = generatePackagingCode(105),
            tankcar = generatePackagingCode(106),
            tanktruck = generatePackagingCode(107),
            split_tank_truck = generatePackagingCode(108),
                //109= generatePackagingCode(109),
            // 110 - 120 organizational packaging
            group = generatePackagingCode(110),
            piece = generatePackagingCode(111),
            assembly = generatePackagingCode(112),
            outfit = generatePackagingCode(113),
            composite = generatePackagingCode(114),
            track = generatePackagingCode(115),
            length = generatePackagingCode(116),
            layer = generatePackagingCode(117),
            mat = generatePackagingCode(118),
            side = generatePackagingCode(119),
            trailer = generatePackagingCode(120),
            shipset=generatePackagingCode(121),
            mixed = generatePackagingCode(122),
            parts = generatePackagingCode(123),
            //ag product blocks
            clove=generatePackagingCode(124),
            bunch=generatePackagingCode(125),
            heart=generatePackagingCode(126),
            loaf=generatePackagingCode(127),

            container = generatePackagingCodeAlternate(0),
            unit = generatePackagingCodeAlternate(1),
            bag = generatePackagingCodeAlternate(2),
            lift = generatePackagingCodeAlternate(3),
            ration = generatePackagingCodeAlternate(4),
            stick = generatePackagingCodeAlternate(5),
            bulk_car = generatePackagingCodeAlternate(6),
            well = generatePackagingCodeAlternate(7),
            lite = generatePackagingCodeAlternate(8),
            leaf = generatePackagingCodeAlternate(9),
            large_spray = generatePackagingCodeAlternate(10),

            bundle = generatePackagingCodeAlternate(11),
            bin = generatePackagingCodeAlternate(12),
            bulk_bag = generatePackagingCodeAlternate(13),
            small_tin = generatePackagingCodeAlternate(14),
            flat = generatePackagingCodeAlternate(15),
            ball = generatePackagingCodeAlternate(16),
            bulk_pack = generatePackagingCodeAlternate(17),
            capsule = generatePackagingCodeAlternate(18),
            assortment = generatePackagingCodeAlternate(19),
            vial = generatePackagingCodeAlternate(20),
            bunk = generatePackagingCodeAlternate(21),
            billet = generatePackagingCodeAlternate(22),
            
            shipment = generatePackagingCodeAlternate(23),
            board = generatePackagingCodeAlternate(24),
            segment = generatePackagingCodeAlternate(25),
            spool = generatePackagingCodeAlternate(26),
            strip = generatePackagingCodeAlternate(27),
            skid = generatePackagingCodeAlternate(28),
            skein = generatePackagingCodeAlternate(29),
            suppository = generatePackagingCodeAlternate(30),
            syringe = generatePackagingCodeAlternate(31),
            straw = generatePackagingCodeAlternate(32),
            
            ocular_insert_system=generatePackagingCodeAlternate(33),
            treatment = generatePackagingCodeAlternate(34),
            tablet = generatePackagingCodeAlternate(35),
            patch = generatePackagingCodeAlternate(36),
            trouche = generatePackagingCodeAlternate(37),
            lozenge = generatePackagingCodeAlternate(38),
            
            wrap = generatePackagingCodeAlternate(39),
            hanging_container = generatePackagingCodeAlternate(40),
            wafer = generatePackagingCodeAlternate(41),
            cast = generatePackagingCodeAlternate(42),
            lift_van = generatePackagingCodeAlternate(43),
            carset = generatePackagingCodeAlternate(44),
            display = generatePackagingCodeAlternate(45),
            card = generatePackagingCodeAlternate(46),
            wheel = generatePackagingCodeAlternate(47),
            cone = generatePackagingCodeAlternate(48),
            coil_group = generatePackagingCodeAlternate(49),
            cylinder = generatePackagingCodeAlternate(50),
            combo = generatePackagingCodeAlternate(51),
            lot = generatePackagingCodeAlternate(52),
            book = generatePackagingCodeAlternate(53),
            block = generatePackagingCodeAlternate(54),
            round = generatePackagingCodeAlternate(55),
            cassette = generatePackagingCodeAlternate(56),
            beam = generatePackagingCodeAlternate(57),
            band = generatePackagingCodeAlternate(58),
            sleeve = generatePackagingCodeAlternate(59),
           
            disk = generatePackagingCodeAlternate(60),
            deal = generatePackagingCodeAlternate(61),
            dispenser = generatePackagingCodeAlternate(62),
            pack = generatePackagingCodeAlternate(63),
            pail = generatePackagingCodeAlternate(64),
            reel = generatePackagingCodeAlternate(65),
            room = generatePackagingCodeAlternate(66),
            session = generatePackagingCodeAlternate(67),
            belt = generatePackagingCodeAlternate(68),
            dose = generatePackagingCodeAlternate(69),
            //
            strand = generatePackagingCodeAlternate(70),
            zone = generatePackagingCodeAlternate(71),
            envelope = generatePackagingCodeAlternate(72),
            blank = generatePackagingCodeAlternate(73),
            head = generatePackagingCodeAlternate(74),
            slice = generatePackagingCodeAlternate(75),
            joint = generatePackagingCodeAlternate(76),
            portion = generatePackagingCodeAlternate(77),
            barge = generatePackagingCodeAlternate(78),
            field = generatePackagingCodeAlternate(79),
            pallet = generatePackagingCodeAlternate(80),
            plate = generatePackagingCodeAlternate(81),
            panel = generatePackagingCodeAlternate(82),
            meal = generatePackagingCodeAlternate(83),
            ring = generatePackagingCodeAlternate(84),
            sack = generatePackagingCodeAlternate(85),
            set = generatePackagingCodeAlternate(86),
            thread = generatePackagingCodeAlternate(87),
            large_tin = generatePackagingCodeAlternate(88),

            particle = generatePackagingCodeAlternate(89),

            sling = generatePackagingCodeAlternate(90),
            line = generatePackagingCodeAlternate(91),
            casing = generatePackagingCodeAlternate(92),
            tote = generatePackagingCodeAlternate(93),
            cask = generatePackagingCodeAlternate(94),
            page = generatePackagingCodeAlternate(95),
            rack = generatePackagingCodeAlternate(96),
            cap = generatePackagingCodeAlternate(97),
            cop = generatePackagingCodeAlternate(98),
            powder_vial = generatePackagingCodeAlternate(99),
            brush = generatePackagingCodeAlternate(100),
            box = generatePackagingCodeAlternate(101),
            bolt = generatePackagingCodeAlternate(102),
            cell = generatePackagingCodeAlternate(103),
            page_electronic = generatePackagingCodeAlternate(104),
            hank = generatePackagingCodeAlternate(105),  // textile
            kit = generatePackagingCodeAlternate(106),
            packet = generatePackagingCodeAlternate(107),
            article = generatePackagingCodeAlternate(108),
            part = generatePackagingCodeAlternate(109),
            parcel = generatePackagingCodeAlternate(110),
            load = generatePackagingCodeAlternate(111),
            lug = generatePackagingCodeAlternate(112),
            square = generatePackagingCodeAlternate(113),
            run = generatePackagingCodeAlternate(114),
            storage_unit = generatePackagingCodeAlternate(115),
            shelf_package = generatePackagingCodeAlternate(116),
            small_spray = generatePackagingCodeAlternate(117),
            heat_lot = generatePackagingCodeAlternate(118),

            batch = generatePackagingCodeAlternate(119),
            pump = generatePackagingCodeAlternate(120),
            stage = generatePackagingCodeAlternate(121),
            cartridge = generatePackagingCodeAlternate(122),
            small_case = generatePackagingCodeAlternate(123),
            leave = generatePackagingCodeAlternate(124),
            cover = generatePackagingCodeAlternate(125),
            syphon = generatePackagingCodeAlternate(125),
            batt = generatePackagingCodeAlternate(126),
            connector = generatePackagingCodeAlternate(127),
        };
    }  // namespace packaging

    // https://en.wikipedia.org/wiki/List_of_traded_commodities
    enum commodity : std::uint32_t {
        water = generateHarmonizedCode(22, 1, 90),
        // metals
        gold = generateHarmonizedCode(71, 8, 0),
        copper = generateHarmonizedCode(74, 0, 0),
        silver = generateHarmonizedCode(71, 6, 0),
        platinum = generateHarmonizedCode(71, 10, 11),
        palladium = generateHarmonizedCode(71, 10, 21),
        uranium = generateHarmonizedCode(26, 12, 0),
        zinc = generateHarmonizedCode(79, 0, 0),
        tin = generateHarmonizedCode(80, 0, 0),
        lead = generateHarmonizedCode(78, 0, 0),
        iron = generateHarmonizedCode(72, 0, 0),
        aluminum = generateHarmonizedCode(76, 0, 0),
        alluminum_alloy = generateHarmonizedCode(76, 01, 20),
        nickel = generateHarmonizedCode(75, 0, 0),
        cobalt = generateHarmonizedCode(81, 05, 0),
        molybdenum = generateHarmonizedCode(81, 02, 0),
        carbon = generateHarmonizedCode(28, 03, 0),

        // energy
        oil = generateHarmonizedCode(27, 9, 0),
        coal = generateHarmonizedCode(27, 11, 0),
        heat_oil = generateHarmonizedCode(27, 10, 19),
        nat_gas = generateHarmonizedCode(27, 11, 21),
        brent_crude = generateHarmonizedCode(27, 9, 0, 10, true),
        ethanol = generateHarmonizedCode(22, 07, 10),
        propane = generateHarmonizedCode(27, 11, 12),
        gasoline = generateHarmonizedCode(27, 10, 12, 15, true),
        // grains
        wheat = generateHarmonizedCode(10, 01, 99),
        corn = generateHarmonizedCode(10, 05, 90),
        soybeans = generateHarmonizedCode(12, 10, 90),
        soybean_meal = generateHarmonizedCode(12, 8, 10),
        soybean_oil = generateHarmonizedCode(15, 07, 00),
        oats = generateHarmonizedCode(10, 04, 90),
        rice = generateHarmonizedCode(10, 06, 00),
        durum_wheat = generateHarmonizedCode(10, 01, 10),
        canola = generateHarmonizedCode(15, 14, 00),
        rough_rice = generateHarmonizedCode(10, 06, 10),
        rapeseed = generateHarmonizedCode(12, 05, 0),
        adzuki = generateHarmonizedCode(07, 13, 32),
        barley = generateHarmonizedCode(10, 03, 90),
        // meats
        live_cattle = generateHarmonizedCode(01, 02, 29),
        feeder_cattle = generateHarmonizedCode(01, 02, 29, 40, true),
        lean_hogs = generateHarmonizedCode(01, 03, 92),
        milk = generateHarmonizedCode(04, 01, 0),
        paper = generateHarmonizedCode(48, 02, 0),
        plastic = generateHarmonizedCode(39, 0, 0),
        // soft
        cotton = generateHarmonizedCode(52, 1, 0),
        orange_juice = generateHarmonizedCode(20, 9, 11),
        sugar = generateHarmonizedCode(17, 01, 0),
        sugar_11 = generateHarmonizedCode(17, 01, 13),
        coffee = generateHarmonizedCode(9, 1, 0),
        cocoa = generateHarmonizedCode(18, 1, 0),
        palm_oil = generateHarmonizedCode(15, 11, 0),
        rubber = generateHarmonizedCode(40, 0, 0),
        wool = generateHarmonizedCode(51, 01, 0),
        lumber = generateHarmonizedCode(44, 00, 0),
        brick = generateHarmonizedCode(68, 10, 11),
        matches = generateHarmonizedCode(36, 05, 00),
        // other common unit blocks
        people = generateKnownCode(15),
        passenger = 115126,
        vehicle = generateHarmonizedCode(87, 3, 0),
        failure = generateStringCode("fail_"),
        freight = 56226,
        labor = generateStringCode("labor"),
        overtime = generateStringCode("extra"),
        doubletime = generateStringCode("2x___"),
        employee = generateStringCode("empl_"),
        visit = generateStringCode("visit"),
        steam = generateStringCode("steam"),
        credit = generateStringCode("cred_"),
        debit = generateStringCode("debit"),
        blank = generateStringCode("blank"),
        job = generateStringCode("job__"),
        trip = generateStringCode("trip_"),
        use = generateStringCode("use__"),
        policy = generateStringCode("geico"),
        night = generateStringCode("night"),
        weekend = generateStringCode("wkend"),
        stops = generateStringCode("stops"),
        metric = generateStringCode("metrc"),
        peak = generateStringCode("peak_"),
        fold = generateStringCode("fold_"),
        seat = generateStringCode("seat_"),
        // clinical
        tissue = generateKnownCode(52632250),
        cell = generateStringCode("cell_"),
        embryo = generateKnownCode(52632253),
        Hahnemann = generateKnownCode(2352622),
        Korsakov = generateKnownCode(26262656),
        protein = generateKnownCode(325255),
        creatinine = generateKnownCode(2566225),
        cartridge_needle = generateStringCode("ctndl"),
        pillow=generateStringCode("pillw"),
        // computer
        pixel = generateStringCode("pixel"),
        dots = generateStringCode("dots_"),
        voxel = generateStringCode("voxel"),
        frame = generateStringCode("frame"),
        errors = generateStringCode("error"),
        character = generateStringCode("ascii"),
        packet=generateStringCode("packt"),
        word = generateStringCode("word_"),
        flop = generateStringCode("flop_"),
        lines_of_code = generateStringCode("loc__"),
        instruction = generateKnownCode(8086),
        information = generateKnownCode(411),
        linesofservice = generateStringCode("telco"),
        port = generateStringCode("port_"),
        reset = generateStringCode("reset"),
        runtime = generateStringCode("time_"),
        record = generateStringCode("data_"),
        test = generateStringCode("test_"),
        message = generateStringCode("msg__"),
        screen = generateStringCode("scrn_"),
        accounting = generateKnownCode(1516115),
        electronic_mail_box = generateStringCode("email"),
        photo=generateStringCode("photo"),
        application=generateStringCode("app__"),
       //projects
        project = generateStringCode("proj_"),
        program = generateStringCode("prog_"),
        task = generateStringCode("task_"),
        session = generateStringCode("sessn"),
        locations = generateStringCode("place"),
        duty = generateStringCode("duty_"),
        // emmissions
        cloud_cover = generateKnownCode(58339544),
        // communications
        call = generateStringCode("call_"),
        person = generateKnownCode(561485),
        impression = generateKnownCode(4616518),
        service = generateStringCode("serve"),
        billboard = generateStringCode("board"),
        channel = generateStringCode("chan_"),
        transactions = generateStringCode("trxn_"),
        // food
        capsaicin = generateKnownCode(623452),
        cigarette = generateHarmonizedCode(24, 02, 20),

        // other
        fiber = generateStringCode("fiber"),
        tires = generateHarmonizedCode(40, 12, 20),
        fissile_material = generateStringCode("nuke_"),
        jewel = generateStringCode("jewel"),
        air = generateStringCode("air__"),
        machine = generateStringCode("mech_"),
        meal = generateStringCode("meal_"),
    };

    enum currencies : std::uint32_t {
        // codes defined in ISO 4217
        dollar = generateCurrencyCode("USD"),
        euro = generateCurrencyCode("EUR"),
        yen = generateCurrencyCode("JPY"),
        pound = generateCurrencyCode("GBP"),
        ruble = generateCurrencyCode("RUB"),
        credits = generateCurrencyCode("XXX"),
    };

    enum chemicals : std::uint32_t {
        KOH = generateHarmonizedCode(28, 15, 20),
        hydrogen_peroxide = generateHarmonizedCode(28, 47, 00),
        nitrogen = generateHarmonizedCode(28, 04, 30),
        methylamine = generateHarmonizedCode(29, 21, 11),
        NaOH = generateHarmonizedCode(28, 15, 12),
        K2O = generateHarmonizedCode(31, 4, 20, 50, false),
        P2O5 = generateHarmonizedCode(28, 9, 0),
        W03 = generateHarmonizedCode(28, 25, 90, 40, false),
        lactose = generateHarmonizedCode(17, 02, 11),
        alcohol = generateHarmonizedCode(22, 07, 10),
        choline_chloride = generateHarmonizedCode(29, 23, 10),
    };

    constexpr std::uint32_t
        package(std::uint32_t packaging, std::uint32_t commodity)
    {
        return packaging | commodity;
    }
}  // namespace commodities

}  // namespace UNITS_NAMESPACE
