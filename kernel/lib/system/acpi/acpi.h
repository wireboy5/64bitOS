#pragma once
#include <stdint.h>
#include <stddef.h>




/*******************************************************************************
 * Definitions of ACPI tables
 ******************************************************************************/


/*
    This is the structure for the RSDP (Root System Description Pointer)
*/
struct ACPI_RSDP {
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdt_address;
} __attribute__ ((packed));
typedef struct ACPI_RSDP ACPI_RSDP_t;

/*
    This is the RSDPv2 structure
*/
struct ACPI_RSDPv2 {
    ACPI_RSDP_t first_part;

    uint32_t length;
    uint64_t xsdt_address;
    uint8_t extended_checksup;
    uint8_t reserved[3];
} __attribute__ ((packed));
typedef struct ACPI_RSDPv2 ACPI_RSDPv2_t;

/*
    This is the header that every SDT table contains.
*/
struct ACPI_SDT_HEADER {
    char signature;
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__ ((packed));


/*
    Structure representing the RSDT (Root System Description Table)
*/
struct ACPI_RSDT {
    struct ACPI_SDT_HEADER header;
    uint32_t *entries;
} __attribute__ ((packed));
typedef struct ACPI_RSDT ACPI_RSDT_t;

/*
    Structure representing the XSDT (eXtended System Description Table)
*/
struct ACPI_XSDT {
    struct ACPI_SDT_HEADER header;
    uint64_t *entries;
} __attribute__ ((packed));
typedef struct ACPI_XSDT ACPI_XSDT_t;

/*
    MADT Entry header
*/
struct ACPI_MADT_entry_header {
    uint8_t type;
    uint8_t length;
} __attribute__ ((packed));
typedef struct ACPI_MADT_entry_header ACPI_MADT_entry_header_t;

/*
    Multiple APIC Description Table
*/
struct ACPI_MADT {
    struct ACPI_SDT_HEADER header;
    uint32_t local_apic_address;
    uint32_t flags;
    ACPI_MADT_entry_header_t* entries;
} __attribute__ ((packed));

/*******************************************************************************
 * MADT Entries
 ******************************************************************************/

#define ACPI_MADT_TYPE_LOCAL_APIC           0
#define ACPI_MADT_TYPE_IO_APIC              1
#define ACPI_MADT_TYPE_INTERRUPT_OVERRIDE   2
#define ACPI_MADT_TYPE_NMI_SOURCE           3
#define ACPI_MADT_TYPE_LOCAL_NMI            4
#define ACPI_MADT_TYPE_LOCAL_ADDR_OVERRIDE  5
#define ACPI_MADT_TYPE_PLOCAL_X2APIC        9


/*
    Processor Local APIC
*/
struct ACPI_MADT_PL_APIC {
    ACPI_MADT_entry_header_t header;
    uint8_t processor_id;
    uint8_t apic_id;
    uint32_t flags;
} __attribute__ ((packed));
typedef struct ACPI_MADT_PL_APIC ACPI_MADT_PL_APIC_t;

/*
    IOAPIC
*/
struct ACPI_MADT_IO_APIC {
    ACPI_MADT_entry_header_t header;
    uint8_t ioapic_id;
    uint8_t reserved;
    uint32_t address;
    uint32_t global_irq_base;
} __attribute__ ((packed));
typedef struct ACPI_MADT_IO_APIC ACPI_MADT_IO_APIC_t;

/*
    IO/APIC Interrupt Source Override
*/
struct ACPI_MADT_IOSO {
    ACPI_MADT_entry_header_t header;
    uint8_t bus_source;
    uint8_t irq_source;
    uint32_t global_irq_base;
    uint16_t flags;
} __attribute__ ((packed));
typedef struct ACPI_MADT_IOSO ACPI_MADT_IOSO_t;

/*
    AIO/APIC Non-maskable interrupt source
*/
struct ACPI_MADT_NMI {
    ACPI_MADT_entry_header_t header;
    uint8_t nmi_source;
    uint8_t reserved;
    uint16_t flags;
    uint32_t global_irq_base;
} __attribute__ ((packed));
typedef struct ACPI_MADT_NMI ACPI_MADT_NMI_t;

/*
    Local APIC Non-maskable interrupts
*/
struct ACPI_MADT_LAPIC_NMI {
    ACPI_MADT_entry_header_t header;
    uint8_t acpi_processor_id;
    uint16_t flags;
    uint8_t lint_no;
} __attribute__ ((packed));
typedef struct ACPI_MADT_LAPIC_NMI ACPI_MADT_LAPIC_NMI_t;

/*
    Local APIC Address Override.
*/
struct ACPI_MADT_LAPIC_ADDR_OVR {
    ACPI_MADT_entry_header_t header;
    uint16_t reserved;
    uint64_t address;
} __attribute__ ((packed));
typedef struct ACPI_MADT_LAPIC_ADDR_OVR ACPI_MADT_LAPIC_ADDR_OVR_t;

/*
    Processor Local x2APIC
*/
struct ACPI_MADT_X2APIC {
    ACPI_MADT_entry_header_t header;
    uint16_t reserved;
    uint32_t X2APIC_ID;
    uint32_t flags;
    uint32_t APIC_ID;
} __attribute__ ((packed));
typedef struct ACPI_MADT_X2APIC ACPI_MADT_X2APIC_t;
